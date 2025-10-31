#include "Room.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <nlohmann/json.hpp>
#include <cstdlib>

using json = nlohmann::json;

Room::Room(float sceneWidth, float sceneHeight, const std::string& configPath) {
    width = sceneWidth;
    height = sceneHeight;

    camera = { 0 };
    camera.target = { width / 2.0f, height / 2.0f };
    camera.offset = { GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    cameraSensitivity = 1.5f;
    cameraSmoothness = 0.05f;

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
}

Room::~Room() {
    UnloadTexture(textureWallTop);
    UnloadTexture(textureWallDown);
    UnloadTexture(textureFloor);
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

std::vector<std::string> Room::GetFurnitureNames() const {
    std::vector<std::string> names;
    for (const auto& furniture : furnitureList) {
        names.push_back(furniture->GetName());
    }
    return names;
}

Furniture* Room::GetFurnitureAtMousePosition() {
    Vector2 mousePosition = GetMousePosition();

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
    Vector2 mousePosition = GetMousePosition();
    float centerX = GetScreenWidth() / 2.0f;
    float screenWidth = GetScreenWidth();

    float mouseOffsetX = 0;
    if (mousePosition.x < screenWidth * 0.3f)
        mouseOffsetX = (mousePosition.x - screenWidth * 0.3f) / centerX;
    else if (mousePosition.x > screenWidth * 0.7f)
        mouseOffsetX = (mousePosition.x - screenWidth * 0.7f) / centerX;

    float targetCameraX = camera.target.x + mouseOffsetX * cameraSensitivity * 200;

    float cameraWidth = GetScreenWidth() / camera.zoom;
    float minCameraX = 0;
    float maxCameraX = width - cameraWidth;

    if (targetCameraX < minCameraX) targetCameraX = minCameraX;
    if (targetCameraX > maxCameraX) targetCameraX = maxCameraX;

    camera.target.x += (targetCameraX - camera.target.x) * cameraSmoothness;

    hoveredFurniture = GetFurnitureAtMousePosition();
}

void Room::Draw() {
    BeginMode2D(camera);

    float percentWallDown = 0.25f;
    float percentFloor = 0.15f;
    float percentWallTop = 1.0f - percentWallDown - percentFloor;

    float heightWallTop = height * percentWallTop;
    float heightWallDown = height * percentWallDown;
    float heightFloor = height * percentFloor;

    struct TextureData {
        Texture2D& texture;
        float height;
        float yPosition;
    };

    TextureData textures[] = {
        {textureWallTop, heightWallTop, 0},
        {textureWallDown, heightWallDown, heightWallTop},
        {textureFloor, heightFloor, heightWallTop + heightWallDown}
    };

    float tileWidths[3];
    float scales[3];
    
    for (int j = 0; j < 3; j++) {
        scales[j] = textures[j].height / textures[j].texture.height;
        tileWidths[j] = textures[j].texture.width * scales[j];
    }

    float minTileWidth = std::min(std::min(tileWidths[0], tileWidths[1]), tileWidths[2]);
    int maxTiles = (int)ceil(width / minTileWidth);

    for (int i = 0; i <= maxTiles; i++) {
        for (int j = 0; j < 3; j++) {
            float x = i * tileWidths[j];
            if (x < width) {
                float currentDestWidth = (x + tileWidths[j] > width) ? width - x : tileWidths[j];
                float sourceWidthRatio = currentDestWidth / tileWidths[j];
                float currentSourceWidth = textures[j].texture.width * sourceWidthRatio;
                
                Rectangle sourceRec;
                Rectangle destRec = {x, textures[j].yPosition, currentDestWidth, textures[j].height};

                if (i % 2 == 1) {
                    sourceRec = {(float)textures[j].texture.width, 0, -currentSourceWidth, (float)textures[j].texture.height};
                } else {
                    sourceRec = {0, 0, currentSourceWidth, (float)textures[j].texture.height};
                }
                
                DrawTexturePro(textures[j].texture, sourceRec, destRec, {0,0}, 0.0f, WHITE);
            }
        }
    }

    float centerX = camera.target.x;
    float tolerance = -0.3f;
    DrawCircle(centerX, camera.target.y, 5, RED);

    for (const auto& furniture : furnitureList) {
        float furnitureX = furniture->GetPosition().x;
        float furnitureWidth = furniture->GetSize().x;

        int side;
        if (centerX < furnitureX - tolerance * furnitureWidth) {
            side = 0;
        } else if (centerX > furnitureX + furnitureWidth + tolerance * furnitureWidth) {
            side = 2;
        } else {
            side = 1;
        }
        
        furniture->Draw(side);
        }

    EndMode2D();

    if (hoveredFurniture != nullptr) {
        Vector2 mousePos = GetMousePosition();
        std::string name = hoveredFurniture->GetName();

        int fontSize = 20;
        int padding = 8;
        Vector2 textSize = MeasureTextEx(font, name.c_str(), fontSize, 1);

        Rectangle bgRect = {
            mousePos.x - padding,
            mousePos.y - textSize.y - padding - 5,
            textSize.x + padding * 2,
            textSize.y + padding * 2
        };
        
        DrawRectangleRec(bgRect, Fade(BLACK, 0.7f));
        DrawRectangleLinesEx(bgRect, 1, WHITE);

        DrawTextEx(font, name.c_str(), 
                  {mousePos.x - textSize.x/2, mousePos.y - textSize.y - padding - 5}, 
                  fontSize, 1, WHITE);
    }
}
