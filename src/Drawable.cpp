#include "Drawable.h"
#include <string>

Drawable::Drawable(const std::string& texturePath) {
    texture = LoadTexture(texturePath.c_str());
    width = texture.width;
    height = texture.height;
}

Drawable::Drawable(const std::string& texturePath, float width, float height) {
    texture = LoadTexture(texturePath.c_str());
    width = width;
    height = height;
}

Drawable::~Drawable() {
    UnloadTexture(texture);
}

void Drawable::Draw(float x, float y) {
    float transformedWidth = scale * width;
    float transformedHeight = scale * height;
    Rectangle destRec = {x, y - transformedHeight, transformedWidth, transformedHeight};
    Rectangle sourceRec = {0, 0, (float)texture.width, (float)texture.height};
    Vector2 origin = {0, 0};

    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

Rectangle Drawable::GetBoundingBox(float x, float y) const {
    return {x, y - height * scale, width * scale, height * scale};
}

