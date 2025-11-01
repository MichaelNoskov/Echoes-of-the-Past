#include "Room.h"
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

Room::Room(float sceneWidth, float sceneHeight, const std::string& configPath, Rectangle area) {
    width = sceneWidth;
    height = sceneHeight;
    lightsOn = true;
    flashlightOn = false;
    drawArea = area;

    camera = { 0 };
    camera.target = { drawArea.width / 2.0f, drawArea.height / 2.0f };
    camera.offset = { drawArea.width / 2.0f + drawArea.x, drawArea.height / 2.0f + drawArea.y };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    cameraSensitivity = 3.0f;
    cameraSmoothness = 5.0f;

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

void Room::ToggleLights() {
    lightsOn = !lightsOn;
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

std::vector<float> Room::getPedestals() {
    std::vector<float> pedestals = {};
    for (const auto& furniture : furnitureList) {
        if (furniture->GetDragging()) {
            continue;
        }
        pedestals.push_back(furniture->GetPosition().y - furniture->GetSize().y);
    }

    std::sort(pedestals.begin(), pedestals.end(), std::less<float>());
    
    return pedestals;
}

bool Room::IsFurnitureOnPedestal(Furniture* furniture) {
    if (!furniture) return false;
    
    Vector2 furniturePos = furniture->GetPosition();
    Vector2 furnitureSize = furniture->GetSize();

    float furnitureBottomY = furniturePos.y;
    float furnitureLeftX = furniturePos.x;
    float furnitureRightX = furniturePos.x + furnitureSize.x;

    for (const auto& otherFurniture : furnitureList) {
        if (otherFurniture.get() == furniture) continue;
        if (otherFurniture->GetDragging()) continue;
        
        Vector2 otherPos = otherFurniture->GetPosition();
        Vector2 otherSize = otherFurniture->GetSize();

        float pedestalTopY = otherPos.y - otherSize.y;

        const float verticalTolerance = 5.0f;
        if (std::abs(furnitureBottomY - pedestalTopY) <= verticalTolerance) {

            const float minOverlapRatio = 0.3f;
            
            float overlapLeft = std::max(furnitureLeftX, otherPos.x);
            float overlapRight = std::min(furnitureRightX, otherPos.x + otherSize.x);
            float overlapWidth = std::max(0.0f, overlapRight - overlapLeft);
            
            float furnitureWidth = furnitureSize.x;
            float overlapRatio = overlapWidth / furnitureWidth;

            if (overlapRatio >= minOverlapRatio) {
                return true;
            }
        }
    }
    
    return false;
}

void Room::Update() {
    Vector2 mousePosition = GetMousePosition();

    float relativeMouseX = mousePosition.x - drawArea.x;
    float centerX = drawArea.width / 2.0f;
    
    float mouseOffsetX = 0;
    if (relativeMouseX < drawArea.width * 0.3f)
        mouseOffsetX = (relativeMouseX - drawArea.width * 0.3f) / centerX;
    else if (relativeMouseX > drawArea.width * 0.7f)
        mouseOffsetX = (relativeMouseX - drawArea.width * 0.7f) / centerX;

    float targetCameraX = camera.target.x + mouseOffsetX * cameraSensitivity * 200;

    float cameraViewWidth = drawArea.width / camera.zoom;

    float minCameraX = drawArea.width / 2.0f;
    float maxCameraX = width - drawArea.width / 2.0f;

    if (targetCameraX < minCameraX) targetCameraX = minCameraX;
    if (targetCameraX > maxCameraX) targetCameraX = maxCameraX;

    camera.target.x += GetFrameTime() * ((targetCameraX - camera.target.x) * cameraSmoothness);

    hoveredFurniture = GetFurnitureAtMousePosition();

    if (lightsOn && hoveredFurniture != nullptr && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && handItem == nullptr) {
        handItem = hoveredFurniture;
        handItem->Drag(true);
        moveToBack(furnitureList, handItem);
    } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && handItem != nullptr && !handItem->GetCollisioning()) {
        handItem->Drag(false);
        handItem = nullptr;
    }

    if (handItem != nullptr) {
        bool collide = false;

        Vector2 furniturePos = handItem->GetPosition();
        Vector2 furnitureSize = handItem->GetSize();
        float furnitureBottomY = furniturePos.y;

        bool onValidPedestal = false;
        
        for (const auto& otherFurniture : furnitureList) {
            if (otherFurniture.get() == handItem) continue;
            if (otherFurniture->GetDragging()) continue;
            
            Vector2 otherPos = otherFurniture->GetPosition();
            Vector2 otherSize = otherFurniture->GetSize();

            float pedestalTopY = otherPos.y - otherSize.y;

            const float verticalTolerance = 5.0f;
            if (std::abs(furnitureBottomY - pedestalTopY) <= verticalTolerance) {

                float furnitureLeftX = furniturePos.x;
                float furnitureRightX = furniturePos.x + furnitureSize.x;
                float pedestalLeftX = otherPos.x;
                float pedestalRightX = otherPos.x + otherSize.x;

                const float minOverlapRatio = 0.7f;
                
                float overlapLeft = std::max(furnitureLeftX, pedestalLeftX);
                float overlapRight = std::min(furnitureRightX, pedestalRightX);
                float overlapWidth = std::max(0.0f, overlapRight - overlapLeft);
                
                float furnitureWidth = furnitureSize.x;
                float overlapRatio = overlapWidth / furnitureWidth;

                if (overlapRatio >= minOverlapRatio) {
                    onValidPedestal = true;
                    break;
                } else {
                    collide = true;
                    break;
                }
            }
        }

        if (!collide) {
            for (const auto& furniture : furnitureList) {
                if (handItem == furniture.get()) continue;
                if (handItem->IntersectsWith(*furniture)) {
                    handItem->Collide(true);
                    collide = true;
                    break;
                }
            }
        }

        if (collide) {
            handItem->Collide(true);
        } else {
            handItem->Collide(false);
        }

        std::vector<float> pedestals = getPedestals();
        Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), camera);

        float posY = height;
        for (float& pedestal : pedestals) {
            if (mouseWorldPos.y < pedestal) {
                posY = pedestal;
                break;
            }
        }

        Vector2 currentFurniturePos = handItem->GetPosition();
        Vector2 currentFurnitureSize = handItem->GetSize();
        handItem->SetPosition(mouseWorldPos.x - currentFurnitureSize.x/2.0f, posY);

        Vector2 itemPos = handItem->GetPosition();
        handItem->SetPosition(std::max(itemPos.x, 0.0f), posY);
        itemPos = handItem->GetPosition();
        Vector2 itemSize = handItem->GetSize();
        handItem->SetPosition(std::min(itemPos.x, width - handItem->GetSize().x), std::max(itemPos.y, itemSize.y));
    }
}

void Room::Draw() {
    BeginScissorMode((int)drawArea.x, (int)drawArea.y, (int)drawArea.width, (int)drawArea.height);

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

        // DrawCircle(furnitureX + 0.2f * furnitureWidth, camera.target.y, 10, RED);
        // DrawCircle(furnitureX + furnitureWidth - 0.2f * furnitureWidth, camera.target.y, 10, RED);
        // DrawCircle(camera.target.x, camera.target.y, 10, GREEN);
        // DrawCircle(drawArea.height/2, drawArea.width/2, 10, DARKBLUE);
    }

    EndMode2D();
    EndScissorMode();
    DrawRectangleLinesEx(drawArea, 5, BLACK);

    if (hoveredFurniture != nullptr && (lightsOn || flashlightOn)) {
        Vector2 mousePos = GetMousePosition();
        std::string name = hoveredFurniture->GetName();

        int fontSize = 20;
        int padding = 8;
        Vector2 textSize = MeasureTextEx(font, name.c_str(), fontSize, 1);

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

        DrawTextEx(font, name.c_str(), 
                {tooltipX + padding, tooltipY + padding}, 
                fontSize, 1, WHITE);
    }
}