#include "Drawable.h"
#include <string>
#include <vector>
#include <algorithm>

Drawable::Drawable(const std::string& texturePath) {
    // Create a map with only "default" state
    std::map<std::string, std::string> texturePaths = {{"default", texturePath}};
    
    // Load textures
    for (const auto& pair : texturePaths) {
        textures[pair.first] = LoadTexture(pair.second.c_str());
        SetTextureFilter(textures[pair.first], TEXTURE_FILTER_POINT);
    }

    // Use the default texture to determine dimensions
    width = textures["default"].width;
    height = textures["default"].height;

    // Create outline textures
    for (const auto& pair : textures) {
        outlineTextures[pair.first] = CreateOutlineTexture(pair.second, width, height);
        SetTextureFilter(outlineTextures[pair.first], TEXTURE_FILTER_POINT);
    }
}

Drawable::Drawable(const std::map<std::string, std::string>& texturePaths) {
    // Validate that "default" state exists
    if (texturePaths.find("default") == texturePaths.end()) {
        // Handle error - you might want to throw an exception or use a fallback
        // For now, we'll use the first texture as default
        currentState = texturePaths.begin()->first;
    }
    
    // Load textures
    for (const auto& pair : texturePaths) {
        textures[pair.first] = LoadTexture(pair.second.c_str());
        SetTextureFilter(textures[pair.first], TEXTURE_FILTER_POINT);
    }

    // Use the default texture to determine dimensions
    width = textures[currentState].width;
    height = textures[currentState].height;

    // Create outline textures
    for (const auto& pair : textures) {
        outlineTextures[pair.first] = CreateOutlineTexture(pair.second, width, height);
        SetTextureFilter(outlineTextures[pair.first], TEXTURE_FILTER_POINT);
    }
}

Drawable::Drawable(const std::string& texturePath, float width, float height) {
    // Create a map with only "default" state
    std::map<std::string, std::string> texturePaths = {{"default", texturePath}};
    
    // Load textures
    for (const auto& pair : texturePaths) {
        textures[pair.first] = LoadTexture(pair.second.c_str());
        SetTextureFilter(textures[pair.first], TEXTURE_FILTER_POINT);
    }

    this->width = width;
    this->height = height;

    // Create outline textures
    for (const auto& pair : textures) {
        outlineTextures[pair.first] = CreateOutlineTexture(pair.second, width, height);
        SetTextureFilter(outlineTextures[pair.first], TEXTURE_FILTER_POINT);
    }
}

Drawable::Drawable(
    const std::map<std::string, std::string>& texturePaths,
    float width,
    float height
) {
    // Validate that "default" state exists
    if (texturePaths.find("default") == texturePaths.end()) {
        // Handle error - you might want to throw an exception or use a fallback
        // For now, we'll use the first texture as default
        currentState = texturePaths.begin()->first;
    }
    
    // Load textures
    for (const auto& pair : texturePaths) {
        textures[pair.first] = LoadTexture(pair.second.c_str());
        SetTextureFilter(textures[pair.first], TEXTURE_FILTER_POINT);
    }

    this->width = width;
    this->height = height;

    // Create outline textures
    for (const auto& pair : textures) {
        outlineTextures[pair.first] = CreateOutlineTexture(pair.second, width, height);
        SetTextureFilter(outlineTextures[pair.first], TEXTURE_FILTER_POINT);
    }
}

Texture2D Drawable::CreateOutlineTexture(Texture2D originalTexture, float targetWidth, float targetHeight, int outlineThickness) {
    Image targetImage = GenImageColor((int)targetWidth, (int)targetHeight, BLANK);

    Image originalImage = LoadImageFromTexture(originalTexture);

    for (int y = 0; y < (int)targetHeight; y++) {
        for (int x = 0; x < (int)targetWidth; x++) {
            int origX = (int)((float)x * originalTexture.width / targetWidth);
            int origY = (int)((float)y * originalTexture.height / targetHeight);
            
            origX = std::min(origX, originalTexture.width - 1);
            origY = std::min(origY, originalTexture.height - 1);
            
            Color color = GetImageColor(originalImage, origX, origY);
            ImageDrawPixel(&targetImage, x, y, color);
        }
    }

    int newWidth = (int)targetWidth + outlineThickness * 2;
    int newHeight = (int)targetHeight + outlineThickness * 2;
    
    Image outlineImage = GenImageColor(newWidth, newHeight, BLANK);
    
    auto isPixelOpaque = [&](int x, int y) -> bool {
        if (x < 0 || x >= (int)targetWidth || y < 0 || y >= (int)targetHeight) 
            return false;
        
        Color color = GetImageColor(targetImage, x, y);
        return color.a > 10;
    };

    for (int y = 0; y < newHeight; y++) {
        for (int x = 0; x < newWidth; x++) {
            int origX = x - outlineThickness;
            int origY = y - outlineThickness;

            if (origX >= 0 && origX < (int)targetWidth && 
                origY >= 0 && origY < (int)targetHeight && 
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

    UnloadImage(originalImage);
    UnloadImage(targetImage);
    UnloadImage(outlineImage);
    
    return outlineTexture;
}

Drawable::~Drawable() {
    // Unload all textures
    for (auto& pair : textures) {
        UnloadTexture(pair.second);
    }
    for (auto& pair : outlineTextures) {
        UnloadTexture(pair.second);
    }
}

void Drawable::Draw(float x, float y) {
    // Get current texture based on state
    Texture2D texture = textures[currentState];

    float transformedWidth = scale * width;
    float transformedHeight = scale * height;
    Rectangle destRec = {x, y - transformedHeight, transformedWidth, transformedHeight};

    Rectangle sourceRec = {0, 0, (float)texture.width, (float)texture.height};
    Vector2 origin = {0, 0};

    DrawTexturePro(texture, sourceRec, destRec, origin, 0.0f, WHITE);
}

void Drawable::DrawOutline(float x, float y, Color outlineColor, float visibility) {
    // Get current textures based on state
    Texture2D texture = textures[currentState];
    Texture2D outlineTexture = outlineTextures[currentState];

    float transformedWidth = scale * width - (outlineTexture.width - scale * width);
    float transformedHeight = scale * height - (outlineTexture.height - scale * height);

    float outlineDrawWidth = scale * width;
    float outlineDrawHeight = scale * height;

    float offsetX = (outlineDrawWidth - transformedWidth) / 2;
    float offsetY = (outlineDrawHeight - transformedHeight) / 2;

    Rectangle outlineDestRec = {x, y - transformedHeight - 2*offsetY, outlineDrawWidth, outlineDrawHeight};
    DrawTexturePro(outlineTexture, {0, 0, (float)outlineTexture.width, (float)outlineTexture.height}, 
                   outlineDestRec, {0, 0}, 0.0f, outlineColor);

    Rectangle destRec = {x + offsetX, y - transformedHeight - offsetY, transformedWidth, transformedHeight};
    Rectangle sourceRec = {0, 0, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, sourceRec, destRec, {0, 0}, 0.0f, Fade(WHITE, visibility));
}

void Drawable::DrawDragging(float x, float y) {
    DrawOutline(x, y, WHITE);
}

void Drawable::DrawCollisioning(float x, float y) {
    DrawOutline(x, y, RED, 0.5f);
}

Rectangle Drawable::GetBoundingBox(float x, float y) const {
    return {x, y - height * scale, width * scale, height * scale};
}

bool Drawable::IsPointInside(float x, float y) const {
    Rectangle bbox = GetBoundingBox(0, 0); // Assuming coordinates are relative
    return x >= bbox.x && x <= bbox.x + bbox.width && 
           y >= bbox.y && y <= bbox.y + bbox.height;
}

// New state management methods
void Drawable::SetState(const std::string& state) {
    if (textures.find(state) != textures.end()) {
        currentState = state;
    }
    // Optionally, you could throw an exception or log a warning here
    // if the state doesn't exist
}

bool Drawable::HasState(const std::string& state) const {
    return textures.find(state) != textures.end();
}