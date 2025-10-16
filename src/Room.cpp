#include "Room.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include <cstdlib>

using json = nlohmann::json;

Room::Room(const std::string& configPath) {
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл конфигурации!" << std::endl;
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

void Room::Draw() {
    float screenWidth = (float)GetScreenWidth();
    float screenHeight = (float)GetScreenHeight();

    float percentWallDown = 0.25f;
    float percentFloor = 0.15f;
    float percentWallTop = 1.0f - percentWallDown - percentFloor;

    Vector2 origin = { 0.0f, 0.0f };

    float heightWallTop = screenHeight * percentWallTop;
    float heightWallDown = screenHeight * percentWallDown;
    float heightFloor = screenHeight * percentFloor;

    Rectangle sourceRecTop = { 0.0f, 0.0f, (float)textureWallTop.width, (float)textureWallTop.height };
    Rectangle destRecTop = { 0.0f, 0.0f, screenWidth, heightWallTop };
    DrawTexturePro(textureWallTop, sourceRecTop, destRecTop, origin, 0.0f, WHITE);

    Rectangle sourceRecDown = { 0.0f, 0.0f, (float)textureWallDown.width, (float)textureWallDown.height };
    Rectangle destRecDown = { 0.0f, heightWallTop, screenWidth, heightWallDown };
    DrawTexturePro(textureWallDown, sourceRecDown, destRecDown, origin, 0.0f, WHITE);

    Rectangle sourceRecFloor = { 0.0f, 0.0f, (float)textureFloor.width, (float)textureFloor.height };
    Rectangle destRecFloor = { 0.0f, heightWallTop + heightWallDown, screenWidth, heightFloor };
    DrawTexturePro(textureFloor, sourceRecFloor, destRecFloor, origin, 0.0f, WHITE);
}

