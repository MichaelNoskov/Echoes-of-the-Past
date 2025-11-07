#include "Clock.h"
#include "Room.h"
#include "Game.h"
#include <cmath>

Clock::Clock(
    const std::string& texturePath,
    const std::string& pendulumTexturePath,
    float pendulumWidth, float pendulumHeight,
    Vector2 pendulumPos, Vector2 arrowPos, float arrowLength,
    float width, float height, float x, float y,
    const std::string& furnitureName,
    bool pedestal, bool canHang
) :
    Furniture(texturePath, width, height, x, y, furnitureName, pedestal, canHang),
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
    bool pedestal, bool canHang
) : Furniture(textureFrontPath, textureLeftPath, textureRightPath, width, height, x, y, furnitureName, pedestal, canHang),
    pendulum({{"default", pendulumTextureFrontPath}, {"left", pendulumTextureLeftPath}, {"right", pendulumTextureRightPath}}, pendulumWidth, pendulumHeight),
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

    Vector2 furniturePos = GetPosition();
    Vector2 furnitureSize = GetSize();

    float textureWidth = 33.0f;
    float textureHeight = 53.0f;

    float scaleX = furnitureSize.x / textureWidth;
    float scaleY = furnitureSize.y / textureHeight;

    Vector2 screenPendulumPos = {
        furniturePos.x + pendulumPosition.x * furnitureSize.x,
        furniturePos.y - furnitureSize.y + pendulumPosition.y * furnitureSize.y + pendulum.GetSize().y
    };

    pendulum.Draw(screenPendulumPos.x, screenPendulumPos.y);

    float time = Furniture::getRoom()->GetBunker()->GetGame()->GetTime();
    float normalizedTime = fmodf(time, 1.0f);

    float angle = (normalizedTime * 2 - 0.25f) * 2.0f * M_PI;

    Vector2 screenArrowPos = {
        furniturePos.x + arrowPosition.x * scaleX,
        furniturePos.y - furnitureSize.y + arrowPosition.y * scaleY
    };

    float screenArrowSize = arrowSize * std::max(scaleX, scaleY);
    float arrowWidth = .05f * screenArrowSize * std::max(scaleX, scaleY);

    Vector2 adjustedStartPos = {
        screenArrowPos.x + cosf(angle) * (arrowWidth / 2.0f),
        screenArrowPos.y + sinf(angle) * (arrowWidth / 2.0f)
    };

    Vector2 endPoint = {
        screenArrowPos.x + cosf(angle) * screenArrowSize,
        screenArrowPos.y + sinf(angle) * screenArrowSize
    };

    DrawLineEx(adjustedStartPos, endPoint, arrowWidth, (Color){ 50, 50, 50, 255 } );
};

std::string Clock::GetText() {
    return Furniture::GetName() + "\n\n" + Furniture::getRoom()->GetBunker()->GetGame()->GetTimeString();
}
