#include "Room.h"
#include "LightFurniture.h"
#include "Bunker.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <algorithm>

using json = nlohmann::json;

void moveToBack(std::vector<std::unique_ptr<Furniture>>& furniture, 
                Furniture* target) {
    if (!target) return;
    
    auto it = std::find_if(furniture.begin(), furniture.end(),
        [target](const std::unique_ptr<Furniture>& ptr) {
            return ptr.get() == target;
        });
    
    if (it != furniture.end() && it != furniture.end() - 1) {
        size_t index = std::distance(furniture.begin(), it);
        std::rotate(it, it + 1, furniture.end());
    }
}

Room::Room(float sceneWidth, float sceneHeight, const std::string& configPath, Rectangle area, Bunker* bunkerRef) {
    width = sceneWidth;
    height = sceneHeight;
    bunker = bunkerRef;

    drawArea = area;

    camera = { 0 };
    camera.target = { drawArea.width / 2.0f, drawArea.height / 2.0f };
    camera.offset = { drawArea.width / 2.0f + drawArea.x, drawArea.height / 2.0f + drawArea.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    cameraSensitivity = 3.0f;
    cameraSmoothness = 5.0f;

    shader = LoadShader(0, "res/shaders/room_shader.fs");
    roomTarget = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл конфигурации: " << configPath << std::endl;
        exit(EXIT_FAILURE);
    }

    json configJson;
    configFile >> configJson;

    std::vector<std::string> wallsTop = configJson["backgrounds"]["wall_top"].get<std::vector<std::string>>();
    std::vector<std::string> wallsDown = configJson["backgrounds"]["wall_down"].get<std::vector<std::string>>();
    std::vector<std::string> floors = configJson["backgrounds"]["floor"].get<std::vector<std::string>>();
    
    if (wallsTop.empty() || wallsDown.empty() || floors.empty()) {
        std::cerr << "Ошибка: в конфиге должно быть указан как минимум один файл по каждой категории фона" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> distTop(0, wallsTop.size() - 1);
    std::uniform_int_distribution<> distDown(0, wallsDown.size() - 1);
    std::uniform_int_distribution<> distFloor(0, floors.size() - 1);

    std::string selectedWallTop = wallsTop[distTop(gen)];
    std::string selectedWallDown = wallsDown[distDown(gen)];
    std::string selectedFloor = floors[distFloor(gen)];

    textureWallTop = LoadTexture(selectedWallTop.c_str());
    textureWallDown = LoadTexture(selectedWallDown.c_str());
    textureFloor = LoadTexture(selectedFloor.c_str());

    font = GetFontDefault();
    hoveredFurniture = nullptr;
    handItem = nullptr;
}

Room::~Room() {
    UnloadTexture(textureWallTop);
    UnloadTexture(textureWallDown);
    UnloadTexture(textureFloor);
    UnloadShader(shader);
    UnloadRenderTexture(roomTarget);
}

void Room::UpdateShaderUniforms() {
    int flashLightsOnLocation = GetShaderLocation(shader, "flashlightOn");
    float flashLightOnValue = flashlightOn ? 1.0f : 0.0f;
    SetShaderValue(shader, flashLightsOnLocation, &flashLightOnValue, SHADER_UNIFORM_FLOAT);

    int lightsOnLocation = GetShaderLocation(shader, "lightsOn");
    float lightsOnValue = lightsOn ? 1.0f : 0.0f;
    SetShaderValue(shader, lightsOnLocation, &lightsOnValue, SHADER_UNIFORM_FLOAT);

    int flashlightPosLoc = GetShaderLocation(shader, "flashlightPos");
    Vector2 mousePos = GetMousePosition();

    Vector2 normalizedPos = {
        (mousePos.x - 100) / GetScreenWidth(),
        1.0f - (mousePos.y - 100) / GetScreenHeight()
    };
    
    SetShaderValue(shader, flashlightPosLoc, &normalizedPos, SHADER_UNIFORM_VEC2);

    float screenSize[2] = { (float)GetScreenWidth(), (float)GetScreenHeight() };
    SetShaderValue(shader, GetShaderLocation(shader, "screenSize"), screenSize, SHADER_UNIFORM_VEC2);
}

void Room::DrawInternal() {
    BeginMode2D(camera);

    DrawRectangle(0, 0, width, height, GRAY);

    float centerX = camera.target.x;

    for (const auto& furniture : furnitureList) {
        float furnitureX = furniture->GetPosition().x;
        float furnitureWidth = furniture->GetSize().x;

        int side;
        if (centerX < furnitureX + 0.2f * furnitureWidth) {
            side = 0;
        } else if (centerX > furnitureX + furnitureWidth - 0.2f * furnitureWidth) {
            side = 2;
        } else {
            side = 1;
        }
        
        furniture->Draw(side);
    }

    EndMode2D();
}

Vector2 Room::ScreenToRoomSpace(Vector2 screenPos) const {
    Vector2 worldPos = GetScreenToWorld2D(screenPos, camera);
    return worldPos;
}

Vector2 Room::RoomToScreenSpace(Vector2 roomPos) const {
    Vector2 screenPos = GetWorldToScreen2D(roomPos, camera);
    return screenPos;
}

void Room::AddFurniture(std::unique_ptr<Furniture> furniture) {
    if (LightFurniture* lamp = dynamic_cast<LightFurniture*>(furniture.get())) {
        SetLights(true);
    }

    furniture->setRoom(this);
    furnitureList.push_back(std::move(furniture));
}

bool Room::RemoveFurniture(const std::string& name) {
    auto it = std::find_if(furnitureList.begin(), furnitureList.end(),
        [&name](const std::unique_ptr<Furniture>& furniture) {
            return furniture->GetName() == name;
        });
    
    if (it != furnitureList.end()) {
        furnitureList.erase(it);
        return true;
    }
    return false;
}

bool Room::RemoveFurniture(int index) {
    if (index >= 0 && index < static_cast<int>(furnitureList.size())) {
        furnitureList.erase(furnitureList.begin() + index);
        return true;
    }
    return false;
}

void Room::ClearAllFurniture() {
    furnitureList.clear();
}

Furniture* Room::GetFurniture(const std::string& name) {
    auto it = std::find_if(furnitureList.begin(), furnitureList.end(),
        [&name](const std::unique_ptr<Furniture>& furniture) {
            return furniture->GetName() == name;
        });
    
    return (it != furnitureList.end()) ? it->get() : nullptr;
}

Furniture* Room::GetFurniture(int index) {
    if (index >= 0 && index < static_cast<int>(furnitureList.size())) {
        return furnitureList[index].get();
    }
    return nullptr;
}

void Room::MoveFurniture(const std::string& name, float newX, float newY) {
    Furniture* furniture = GetFurniture(name);
    if (furniture) {
        furniture->SetPosition(newX, newY);
    }
}

void Room::SetLights(bool on) {
    Resource* energy = bunker->GetResource("Energy");
    if (on && energy && energy->GetValue() <= 0) {
        on = false;
    }
    lightsOn = on;
}

void Room::ToggleLights() {
    SetLights(!lightsOn);
}

void Room::ToggleFlashLight() {
    flashlightOn = !flashlightOn;
}

void Room::SetDrawArea(Rectangle area) {
    drawArea = area;
    camera.target = { drawArea.width / 2.0f, drawArea.height / 2.0f };
    camera.offset = { drawArea.width / 2.0f + drawArea.x, drawArea.height / 2.0f + drawArea.y };

    float minCameraX = drawArea.width / 2.0f;
    float maxCameraX = width - drawArea.width / 2.0f;

    if (camera.target.x < minCameraX) camera.target.x = minCameraX;
    if (camera.target.x > maxCameraX) camera.target.x = maxCameraX;
}

Rectangle Room::GetDrawArea() const {
    return drawArea;
}

std::vector<std::string> Room::GetFurnitureNames() const {
    std::vector<std::string> names;
    for (const auto& furniture : furnitureList) {
        names.push_back(furniture->GetName());
    }
    return names;
}

Furniture* Room::GetFurnitureAtMousePosition() {
    Vector2 mousePosition = GetMousePosition();

    if (!CheckCollisionPointRec(mousePosition, drawArea)) {
        return nullptr;
    }

    Vector2 worldPos = GetScreenToWorld2D(mousePosition, camera);

    for (const auto& furniture : furnitureList) {
        Rectangle bbox = furniture->GetBoundingBox();

        if (CheckCollisionPointRec(worldPos, bbox)) {
            return furniture.get();
        }
    }

    return nullptr;
}

void Room::Update() {
    const Vector2 mousePosition = GetMousePosition();

    const float relativeMouseX = mousePosition.x - drawArea.x;
    const float centerX = drawArea.width * 0.5f;
    
    float mouseOffsetX = 0.0f;
    const float lowThreshold = drawArea.width * 0.2f;
    const float highThreshold = drawArea.width * 0.8f;
    
    if (relativeMouseX < lowThreshold) {
        mouseOffsetX = (relativeMouseX - lowThreshold) / centerX;
    } else if (relativeMouseX > highThreshold) {
        mouseOffsetX = (relativeMouseX - highThreshold) / centerX;
    }

    const float cameraViewWidth = drawArea.width / camera.zoom;
    const float minCameraX = drawArea.width * 0.5f;
    const float maxCameraX = width - minCameraX;

    float targetCameraX = camera.target.x + mouseOffsetX * cameraSensitivity * 200.0f;
    targetCameraX = std::clamp(targetCameraX, minCameraX, maxCameraX);

    const float frameTime = GetFrameTime();
    camera.target.x += frameTime * ((targetCameraX - camera.target.x) * cameraSmoothness);

    hoveredFurniture = GetFurnitureAtMousePosition();

    const bool leftMousePressed = IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    
    if (lightsOn && hoveredFurniture && leftMousePressed && !handItem && !hoveredFurniture->GetFreeze()) {
        handItem = hoveredFurniture;
        handItem->Drag(true);
        if (handItem -> GetOnPedestal()) handItem->GetPedestal()->removeItem();
        moveToBack(furnitureList, handItem);
    } else if (leftMousePressed && handItem && !handItem->GetCollisioning()) {
        handItem->Drag(false);
        if (handItem -> GetOnPedestal()) handItem->GetPedestal()->addItem();
        handItem = nullptr;
    }

    if (handItem) {
        bool collide = false;
        float posY = height;

        if (handItem->GetOnPedestal()) {
            const Vector2 itemPosition = handItem->GetPosition();
            const float itemWidth = handItem->GetSize().x;
            Furniture* pedestal = handItem->GetPedestal();
            const float pedestalWidth = pedestal->GetSize().x;
            const float pedestalX = pedestal->GetPosition().x;
            
            posY = itemPosition.y;
            
            if (itemPosition.x + itemWidth < pedestalX || itemPosition.x > pedestalX + pedestalWidth) {
                handItem->setPedestal(nullptr);
            } else if (itemPosition.x + itemWidth * 0.25f < pedestalX || 
                      itemPosition.x + itemWidth * 0.75f > pedestalX + pedestalWidth) {
                collide = true;
            }
        }

        for (const auto& furniture : furnitureList) {
            if (handItem == furniture.get()) continue;
            
            if (handItem->IntersectsWith(*furniture)) {
                if (furniture->IsPedestal()){
                    posY = furniture->GetPosition().y - furniture->GetSize().y;
                    handItem->setPedestal(furniture.get());
                }
                collide = true;
                break;
            }
        }

        handItem->Collide(collide);

        const Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);
        const Vector2 furnitureSize = handItem->GetSize();

        if (handItem->CanHang()) {
            posY = mouseWorldPos.y + furnitureSize.y * 0.3f;
        }
        
        float targetX = mouseWorldPos.x - furnitureSize.x * 0.7f;
        targetX = std::clamp(targetX, 0.0f, width - furnitureSize.x);
        
        float targetY = std::clamp(posY, furnitureSize.y, height);
        handItem->SetPosition(targetX, targetY);
    }
    for (const auto& furniture : furnitureList) {
        furniture -> Update(hoveredFurniture == furniture.get());
    }
}


void Room::Draw() {
    BeginTextureMode(roomTarget);
        ClearBackground(BLACK);
        DrawInternal();
        DrawRectangleLinesEx(drawArea, 5, BLACK);
    EndTextureMode();

    UpdateShaderUniforms();

    BeginScissorMode((int)drawArea.x, (int)drawArea.y, (int)drawArea.width, (int)drawArea.height);
    BeginShaderMode(shader);
        DrawTextureRec(roomTarget.texture, 
            (Rectangle){0, 0, (float)roomTarget.texture.width, (float)-roomTarget.texture.height},
            (Vector2){0, 0}, WHITE);
    EndShaderMode();
    EndScissorMode();

    if (hoveredFurniture != nullptr && (lightsOn || flashlightOn)) {
        Vector2 mousePos = GetMousePosition();
        std::string signature = hoveredFurniture->GetText();

        int fontSize = 20;
        int padding = 8;
        Vector2 textSize = MeasureTextEx(font, signature.c_str(), fontSize, 1);

        float tooltipX = mousePos.x;
        float tooltipY = mousePos.y - textSize.y - padding - 5;

        if (tooltipY < drawArea.y) {
            tooltipY = mousePos.y + 20;
        }
        if (tooltipX + textSize.x + padding * 2 > drawArea.x + drawArea.width) {
            tooltipX = drawArea.x + drawArea.width - textSize.x - padding * 2;
        }
        if (tooltipX < drawArea.x) {
            tooltipX = drawArea.x;
        }

        Rectangle bgRect = {
            tooltipX,
            tooltipY,
            textSize.x + padding * 2,
            textSize.y + padding * 2
        };
        
        DrawRectangleRec(bgRect, Fade(BLACK, 0.7f));
        DrawRectangleLinesEx(bgRect, 1, WHITE);

        DrawTextEx(font, signature.c_str(), 
                {tooltipX + padding, tooltipY + padding}, 
                fontSize, 1, WHITE);
    }
}

