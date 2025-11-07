#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "raylib.h"
#include <string>
#include <vector>
#include <map>

class Drawable {
private:
    float width;
    float height;
    float scale = 1.0f;

    std::map<std::string, Texture2D> textures;
    std::map<std::string, Texture2D> outlineTextures;
    std::string currentState = "default";

    void DrawOutline(float x, float y, Color outlineColor, float visibility = 1);
    Texture2D CreateOutlineTexture(Texture2D originalTexture, float targetWidth, float targetHeight, int outlineThickness = 10);

public:
    Drawable(
            const std::string& texturePath
    );
    Drawable(
        const std::map<std::string, std::string>& texturePaths
    );
    Drawable(
        const std::string& texturePath,
        float width,
        float height
    );
    Drawable(
        const std::map<std::string, std::string>& texturePaths,
        float width,
        float height
    );
    ~Drawable();

    void Draw(float x, float y);
    void DrawDragging(float x, float y);
    void DrawCollisioning(float x, float y);
    void SetScale(float newScale) { scale = newScale; };
    float GetScale() const { return scale; };
    Vector2 GetSize() const { return {width * scale, height * scale}; };
    Rectangle GetBoundingBox(float x, float y) const;
    bool IsPointInside(float x, float y) const;
    
    // New methods for state management
    void SetState(const std::string& state);
    std::string GetCurrentState() const { return currentState; };
    bool HasState(const std::string& state) const;
};

#endif