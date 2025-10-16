#include "Room.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <nlohmann/json.hpp>
#include <cstdlib>

using json = nlohmann::json;
Room::Room(float sceneWidth, float sceneHeigth, const std::string& configPath) {
    width = sceneWidth;
    heigth = sceneHeigth;

    camera = { 0 };
    camera.target = { width / 2.0f, 0 };
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
}

Room::~Room() {
    UnloadTexture(textureWallTop);
    UnloadTexture(textureWallDown);
    UnloadTexture(textureFloor);
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
}

void Room::Draw() {
    BeginMode2D(camera);

    float percentWallDown = 0.25f;
    float percentFloor = 0.15f;
    float percentWallTop = 1.0f - percentWallDown - percentFloor;

    Vector2 origin = { 0.0f, 0.0f };

    float heightWallTop = heigth * percentWallTop;
    float heightWallDown = heigth * percentWallDown;
    float heightFloor = heigth * percentFloor;

    Rectangle sourceRecTop = { 0.0f, 0.0f, (float)textureWallTop.width, (float)textureWallTop.height };
    Rectangle destRecTop = { 0.0f, 0.0f, width, heightWallTop };
    DrawTexturePro(textureWallTop, sourceRecTop, destRecTop, origin, 0.0f, WHITE);

    Rectangle sourceRecDown = { 0.0f, 0.0f, (float)textureWallDown.width, (float)textureWallDown.height };
    Rectangle destRecDown = { 0.0f, heightWallTop, width, heightWallDown };
    DrawTexturePro(textureWallDown, sourceRecDown, destRecDown, origin, 0.0f, WHITE);

    Rectangle sourceRecFloor = { 0.0f, 0.0f, (float)textureFloor.width, (float)textureFloor.height };
    Rectangle destRecFloor = { 0.0f, heightWallTop + heightWallDown, width, heightFloor };
    DrawTexturePro(textureFloor, sourceRecFloor, destRecFloor, origin, 0.0f, WHITE);

    EndMode2D();
}