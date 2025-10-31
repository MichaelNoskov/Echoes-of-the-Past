#ifndef FURNITURE_H
#define FURNITURE_H

#include "raylib.h"
#include "Drawable.h"
#include <string>

class Furniture {
private:
    float posX;
    float posY;

    Drawable surface;

    std::string name;

public:
    Furniture(
        const std::string& texturePath,
        float width, float height, float x = 0.0f, float y = 0.0f, const std::string& furnitureName = ""
    );
    Furniture(
        const std::string& textureFrontPath,
        const std::string& textureLeftPath,
        const std::string& textureRightPath,
        float width, float height, float x = 0.0f, float y = 0.0f, const std::string& furnitureName = ""
    );
    ~Furniture();

    void Draw();
    void SetPosition(float x, float y) { posX = x; posY = y; }
    Vector2 GetPosition() const { return {posX, posY}; }
    std::string GetName() const { return name; }
    Rectangle GetBoundingBox() const;
    bool IsPointInside(float x, float y) const;
};

#endif