#include "Room.h"
#include <fstream>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Room::Room(const std::string& configPath) {
    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        background = LoadTexture("res/default.png");
        return;
    }

    json configJson;
    configFile >> configJson;

    std::vector<std::string> backgrounds = configJson["backgrounds"].get<std::vector<std::string>>();
    if (backgrounds.empty()) {
        background = LoadTexture("res/default.png");
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, backgrounds.size() - 1);
    std::string selectedBackground = backgrounds[dist(gen)];

    background = LoadTexture(selectedBackground.c_str());
}

Room::~Room() {
    UnloadTexture(background);
}

void Room::Draw() {
    Rectangle sourceRec = { 0.0f, 0.0f, (float)background.width, (float)background.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)GetScreenWidth(), (float)GetScreenHeight() };
    Vector2 origin = { 0.0f, 0.0f };

    DrawTexturePro(background, sourceRec, destRec, origin, 0.0f, WHITE);
}
