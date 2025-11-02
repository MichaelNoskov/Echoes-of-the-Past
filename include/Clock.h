#ifndef CLOCK_H
#define CLOCK_H

#include "Furniture.h"

class Room;

class Clock : public Furniture {

private:
    Drawable pendulum;
    Vector2 pendulumPosition;

    float arrowSize = 1;
    Vector2 arrowPosition;

public:
    Clock(
        const std::string& texturePath,
        const std::string& pendulumTexturePath,
        float pendulumWidth, float pendulumHeight,
        Vector2 pendulumPos, Vector2 arrowPos, float arrowLength,
        float width, float height, float x = 0.0f, float y = 0.0f,
        const std::string& furnitureName = "",
        bool pedestal = false, bool canHang = false
    );

    Clock(
        const std::string& textureFrontPath,
        const std::string& textureLeftPath,
        const std::string& textureRightPath,
        const std::string& pendulumTextureFrontPath,
        const std::string& pendulumTextureLeftPath,
        const std::string& pendulumTextureRightPath,
        float pendulumWidth, float pendulumHeight,
        Vector2 pendulumPos, Vector2 arrowPos, float arrowLength,
        float width, float height, float x = 0.0f, float y = 0.0f,
        const std::string& furnitureName = "",
        bool pedestal = false, bool canHang = false
    );
    
    void Update(bool interact) override;
    void Draw(int side = 1);
    std::string GetText() override;
};

#endif