#include "Furniture.h"
#include "Drawable.h"


Furniture::Furniture(
    const std::string& texturePath,
    float width,
    float height,
    float x,
    float y,
    const std::string& furnitureName
) : surface(texturePath, width, height), posX(x), posY(y), name(furnitureName) {
}

Furniture::Furniture(
    const std::string& textureFrontPath,
    const std::string& textureLeftPath,
    const std::string& textureRightPath,
    float width,
    float height,
    float x,
    float y,
    const std::string& furnitureName
) : surface(textureFrontPath, textureLeftPath, textureRightPath, width, height), posX(x), posY(y), name(furnitureName) {
}

Furniture::~Furniture() {
}

void Furniture::Draw(int side) {
    if (isCollisioning) {
        surface.DrawCollisioning(posX, posY, side);
    } else if (isDragging) {
        surface.DrawDragging(posX, posY, side);
    } else {
        surface.Draw(posX, posY, side);
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
