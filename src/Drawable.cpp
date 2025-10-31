#include "Drawable.h"
#include <string>

Drawable::Drawable(const std::string& texturePath) {
    textureFront = LoadTexture(texturePath.c_str());
    textureLeft = LoadTexture(texturePath.c_str());
    textureRight = LoadTexture(texturePath.c_str());

    width = textureFront.width;
    height = textureFront.height;
}

Drawable::Drawable(
    const std::string& textureFrontPath,
    const std::string& textureLeftPath,
    const std::string& textureRightPath
) {
    textureFront = LoadTexture(textureFrontPath.c_str());
    textureLeft = LoadTexture(textureLeftPath.c_str());
    textureRight = LoadTexture(textureRightPath.c_str());

    width = textureFront.width;
    height = textureFront.height;
}

Drawable::Drawable(const std::string& texturePath, float width, float height) {
    textureFront = LoadTexture(texturePath.c_str());
    textureLeft = LoadTexture(texturePath.c_str());
    textureRight = LoadTexture(texturePath.c_str());
    this->width = width;
    this->height = height;
}

Drawable::Drawable(
    const std::string& textureFrontPath,
    const std::string& textureLeftPath,
    const std::string& textureRightPath,
    float width,
    float height
) {
    textureFront = LoadTexture(textureFrontPath.c_str());
    textureLeft = LoadTexture(textureLeftPath.c_str());
    textureRight = LoadTexture(textureRightPath.c_str());
    this->width = width;
    this->height = height;
}

Drawable::~Drawable() {
    UnloadTexture(textureFront);
    UnloadTexture(textureLeft);
    UnloadTexture(textureRight);
}

void Drawable::Draw(float x, float y, int side) {
    Texture2D texture = textureFront;
    if (side == 0) {
        texture = textureLeft;
    } else if (side == 2) {
        texture = textureRight;
    }

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
