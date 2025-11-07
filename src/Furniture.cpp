#include "Furniture.h"
#include "Drawable.h"
#include "Room.h"


Furniture::Furniture(
    const std::string& texturePath,
    float width,
    float height,
    float x,
    float y,
    const std::string& furnitureName,
    bool pedestal, bool canHang
) : surface(texturePath, width, height), posX(x), posY(y), name(furnitureName), isPedestal(pedestal), hang(canHang) {
}

Furniture::Furniture(
    const std::string& textureFrontPath,
    const std::string& textureLeftPath,
    const std::string& textureRightPath,
    float width,
    float height,
    float x,
    float y,
    const std::string& furnitureName,
    bool pedestal, bool canHang
) : surface({{"default", textureFrontPath}, {"left", textureLeftPath}, {"right", textureRightPath}}, width, height), posX(x), posY(y), name(furnitureName), isPedestal(pedestal), hang(canHang) {
}

Furniture::Furniture(
    const std::map<std::string, std::string>& texturePaths,
    float width, float height, float x, float y, const std::string& furnitureName, bool pedestal, bool canHang
) : surface(texturePaths, width, height), posX(x), posY(y), name(furnitureName), isPedestal(pedestal), hang(canHang) {
}

Furniture::~Furniture() {
}

void Furniture::Update(bool interact){
}

void Furniture::Draw(int side) {

    std::string state = "default";
    if (side == 0) {
        state = "left";
    } else if (side == 2) {
        state = "right";
    }
    surface.SetState(state);

    if (isCollisioning) {
        surface.DrawCollisioning(posX, posY);
    } else if (isDragging) {
        surface.DrawDragging(posX, posY);
    } else {
        surface.Draw(posX, posY);
    }
}

Rectangle Furniture::GetBoundingBox() const {
    return surface.GetBoundingBox(posX, posY);
}

bool Furniture::IsPointInside(float x, float y) const {
    Rectangle bbox = GetBoundingBox();
    return x >= bbox.x && x <= bbox.x + bbox.width && 
           y >= bbox.y && y <= bbox.y + bbox.height;
}

bool Furniture::IntersectsWithArea(const Rectangle& area) const {
        Rectangle myBBox = GetBoundingBox();
        return CheckCollisionRecs(myBBox, area);
    }

bool Furniture::IntersectsWith(const Furniture& other) const {
    Rectangle myBBox = GetBoundingBox();
    Rectangle otherBBox = other.GetBoundingBox();
    return CheckCollisionRecs(myBBox, otherBBox);
}
