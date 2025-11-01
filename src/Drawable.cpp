#include "Drawable.h"
#include <string>
#include <vector>
#include <algorithm>

Drawable::Drawable(const std::string& texturePath) {
    textureFront = LoadTexture(texturePath.c_str());
    textureLeft = LoadTexture(texturePath.c_str());
    textureRight = LoadTexture(texturePath.c_str());

    SetTextureFilter(textureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureRight, TEXTURE_FILTER_POINT);

    width = textureFront.width;
    height = textureFront.height;

    outlineTextureFront = CreateOutlineTexture(textureFront);
    outlineTextureLeft = CreateOutlineTexture(textureLeft);
    outlineTextureRight = CreateOutlineTexture(textureRight);
    
    SetTextureFilter(outlineTextureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureRight, TEXTURE_FILTER_POINT);
}

Drawable::Drawable(
    const std::string& textureFrontPath,
    const std::string& textureLeftPath,
    const std::string& textureRightPath
) {
    textureFront = LoadTexture(textureFrontPath.c_str());
    textureLeft = LoadTexture(textureLeftPath.c_str());
    textureRight = LoadTexture(textureRightPath.c_str());

    SetTextureFilter(textureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureRight, TEXTURE_FILTER_POINT);

    width = textureFront.width;
    height = textureFront.height;

    outlineTextureFront = CreateOutlineTexture(textureFront);
    outlineTextureLeft = CreateOutlineTexture(textureLeft);
    outlineTextureRight = CreateOutlineTexture(textureRight);
    
    SetTextureFilter(outlineTextureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureRight, TEXTURE_FILTER_POINT);
}

Drawable::Drawable(const std::string& texturePath, float width, float height) {
    textureFront = LoadTexture(texturePath.c_str());
    textureLeft = LoadTexture(texturePath.c_str());
    textureRight = LoadTexture(texturePath.c_str());

    SetTextureFilter(textureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureRight, TEXTURE_FILTER_POINT);

    this->width = width;
    this->height = height;

    outlineTextureFront = CreateOutlineTexture(textureFront);
    outlineTextureLeft = CreateOutlineTexture(textureLeft);
    outlineTextureRight = CreateOutlineTexture(textureRight);
    
    SetTextureFilter(outlineTextureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureRight, TEXTURE_FILTER_POINT);
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

    SetTextureFilter(textureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(textureRight, TEXTURE_FILTER_POINT);

    this->width = width;
    this->height = height;

    outlineTextureFront = CreateOutlineTexture(textureFront);
    outlineTextureLeft = CreateOutlineTexture(textureLeft);
    outlineTextureRight = CreateOutlineTexture(textureRight);
    
    SetTextureFilter(outlineTextureFront, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureLeft, TEXTURE_FILTER_POINT);
    SetTextureFilter(outlineTextureRight, TEXTURE_FILTER_POINT);
}

Texture2D Drawable::CreateOutlineTexture(Texture2D originalTexture, int outlineThickness) {
    Image image = LoadImageFromTexture(originalTexture);

    int newWidth = originalTexture.width + outlineThickness * 2;
    int newHeight = originalTexture.height + outlineThickness * 2;
    
    Image outlineImage = GenImageColor(newWidth, newHeight, BLANK);
    
    auto isPixelOpaque = [&](int x, int y) -> bool {
        if (x < 0 || x >= originalTexture.width || y < 0 || y >= originalTexture.height) 
            return false;
        
        Color color = GetImageColor(image, x, y);
        return color.a > 10;
    };

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int origX = x - outlineThickness;
            int origY = y - outlineThickness;

            if (origX >= 0 && origX < originalTexture.width && 
                origY >= 0 && origY < originalTexture.height && 
                isPixelOpaque(origX, origY)) {
                continue;
            }

            bool isOutline = false;
            for (int dy = -outlineThickness; dy <= outlineThickness && !isOutline; dy++) {
                for (int dx = -outlineThickness; dx <= outlineThickness && !isOutline; dx++) {
                    if (dx == 0 && dy == 0) continue;
                    
                    int neighborX = origX + dx;
                    int neighborY = origY + dy;
                    
                    if (isPixelOpaque(neighborX, neighborY)) {
                        isOutline = true;
                    }
                }
            }
            
            if (isOutline) {
                ImageDrawPixel(&outlineImage, x, y, {255, 255, 255, 255});
            }
        }
    }

    Texture2D outlineTexture = LoadTextureFromImage(outlineImage);

    UnloadImage(image);
    UnloadImage(outlineImage);
    
    return outlineTexture;
}
Drawable::~Drawable() {
    UnloadTexture(textureFront);
    UnloadTexture(textureLeft);
    UnloadTexture(textureRight);
    UnloadTexture(outlineTextureFront);
    UnloadTexture(outlineTextureLeft);
    UnloadTexture(outlineTextureRight);
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

void Drawable::DrawOutline(float x, float y, int side, Color outlineColor, float visibility) {
    Texture2D texture = textureFront;
    Texture2D outlineTexture = outlineTextureFront;
    
    if (side == 0) {
        texture = textureLeft;
        outlineTexture = outlineTextureLeft;
    } else if (side == 2) {
        texture = textureRight;
        outlineTexture = outlineTextureRight;
    }

    float transformedWidth = scale * width;
    float transformedHeight = scale * height;

    Rectangle destRec = {x, y - transformedHeight, transformedWidth, transformedHeight};
    DrawTexturePro(texture, {0, 0, (float)texture.width, (float)texture.height}, 
                    destRec, {0, 0}, 0.0f, Fade(WHITE, visibility));

    DrawTexturePro(outlineTexture, {0, 0, (float)outlineTexture.width, (float)outlineTexture.height}, 
                   destRec, {0, 0}, 0.0f, outlineColor);
}

void Drawable::DrawDragging(float x, float y, int side) {
    DrawOutline(x, y, side, WHITE);
}

void Drawable::DrawCollisioning(float x, float y, int side) {
    DrawOutline(x, y, side, RED, 0.5f);
}

Rectangle Drawable::GetBoundingBox(float x, float y) const {
    return {x, y - height * scale, width * scale, height * scale};
}