#include "Furniture.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <nlohmann/json.hpp>
#include <cstdlib>

using json = nlohmann::json;

Furniture::Furniture(const std::string& texturePath, float fWidth, float fHeight, float x, float y, const std::string& furnitureName) {
    texture = LoadTexture(texturePath.c_str());
    width = fWidth;
    height = fHeight;
    posX = x;
    posY = y;
    name = furnitureName;
}

Furniture::~Furniture() {
    UnloadTexture(texture);
}

void Furniture::Draw() {
    Rectangle destRec = {posX, posY - height, width, height};
    Rectangle sourceRec = {0, 0, (float)texture.width, (float)texture.height};
    Vector2 origin = {0, 0};
    
    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

Rectangle Furniture::GetBoundingBox() const {
    return {posX, posY - height, width, height};
}

BoundingBox Furniture::GetBoundingBox3D() const {
    BoundingBox box;
    box.min = { posX, posY - height, -0.1f };
    box.max = { posX + width, posY, 0.1f };
    return box;
}

bool Furniture::IsPointInside(float x, float y) const {
    Rectangle bbox = GetBoundingBox();
    return x >= bbox.x && x <= bbox.x + bbox.width && 
           y >= bbox.y && y <= bbox.y + bbox.height;
}
