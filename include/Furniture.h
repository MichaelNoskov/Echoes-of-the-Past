#ifndef FURNITURE_H
#define FURNITURE_H

#include "raylib.h"
#include <string>

class Furniture {
private:
    float width;
    float height;
    float posX;
    float posY;
    Texture2D texture;
    std::string name;

public:
    Furniture(const std::string& texturePath, float fWidth = 10.0f, float fHeight = 10.0f, float x = 0.0f, float y = 0.0f, const std::string& furnitureName = "");
    ~Furniture();

    void Draw();
    void SetPosition(float x, float y) { posX = x; posY = y; }
    Vector2 GetPosition() const { return {posX, posY}; }
    std::string GetName() const { return name; }
    Rectangle GetBoundingBox() const;
};

#endif