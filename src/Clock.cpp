#include "Clock.h"
#include "Room.h"
#include "Game.h"

Clock::Clock(
    const std::string& texturePath,
    const std::string& pendulumTexturePath,
    float pendulumWidth, float pendulumHeight,
    Vector2 pendulumPos, Vector2 arrowPos, float arrowLength,
    float width, float height, float x, float y,
    const std::string& furnitureName,
    bool pedestal
) :
    Furniture(texturePath, width, height, x, y, furnitureName, pedestal),
    pendulum(pendulumTexturePath, pendulumWidth, pendulumHeight),
    pendulumPosition(pendulumPos),
    arrowPosition(arrowPos),
    arrowSize(arrowLength)
{
}

Clock::Clock(
    const std::string& textureFrontPath,
    const std::string& textureLeftPath,
    const std::string& textureRightPath,
    const std::string& pendulumTextureFrontPath,
    const std::string& pendulumTextureLeftPath,
    const std::string& pendulumTextureRightPath,
    float pendulumWidth, float pendulumHeight,
    Vector2 pendulumPos, Vector2 arrowPos, float arrowLength,
    float width, float height, float x, float y,
    const std::string& furnitureName,
    bool pedestal
) : Furniture(textureFrontPath, textureLeftPath, textureRightPath, width, height, x, y, furnitureName, pedestal),
    pendulum(pendulumTextureFrontPath, pendulumTextureLeftPath, pendulumTextureRightPath, pendulumWidth, pendulumHeight),
    pendulumPosition(pendulumPos),
    arrowPosition(arrowPos),
    arrowSize(arrowLength)  
{
}

void Clock::Update(bool interact) {
    Furniture::Update(interact);
}

void Clock::Draw(int side) {
    Furniture::Draw(side);
    pendulum.Draw(pendulumPosition.x, pendulumPosition.y, side);
};

std::string Clock::GetText() {
    return Furniture::GetName() + "\n\n======\n" + Furniture::getRoom()->GetGame()->GetTimeString() + "\n======";
}
