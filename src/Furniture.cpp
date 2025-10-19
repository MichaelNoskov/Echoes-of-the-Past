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

Furniture::~Furniture() {
}

void Furniture::Draw() {
    surface.Draw(posX, posY);
}

Rectangle Furniture::GetBoundingBox() const {
    return surface.GetBoundingBox(posX, posY);
}

bool Furniture::IsPointInside(float x, float y) const {
    Rectangle bbox = GetBoundingBox();
    return x >= bbox.x && x <= bbox.x + bbox.width && 
           y >= bbox.y && y <= bbox.y + bbox.height;
}
