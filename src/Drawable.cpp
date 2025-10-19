#include "Drawable.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <nlohmann/json.hpp>
#include <cstdlib>

using json = nlohmann::json;

Drawable::Drawable(const std::string& texturePath, float width, float height) {
    texture = LoadTexture(texturePath.c_str());
    width = width;
    height = height;
}

Drawable::Drawable(const std::string& texturePath) {
    texture = LoadTexture(texturePath.c_str());
    width = texture.width;
    height = texture.height;
}

Drawable::~Drawable() {
    UnloadTexture(texture);
}

void Drawable::Draw(float x, float y) {
    Rectangle destRec = {x, y - height, width, height};
    Rectangle sourceRec = {0, 0, (float)texture.width, (float)texture.height};
    Vector2 origin = {0, 0};

    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}
