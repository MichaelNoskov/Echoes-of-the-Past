#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "raylib.h"
#include <string>
#include <vector>

class Drawable {
private:
    float width;
    float height;
    float scale = 1.0f;

    Texture2D textureFront, textureLeft, textureRight;
    Texture2D outlineTextureFront, outlineTextureLeft, outlineTextureRight;

    void DrawOutline(float x, float y, int side, Color outlineColor, float visibility = 1);
    Texture2D CreateOutlineTexture(Texture2D originalTexture);

public:
    Drawable(
            const std::string& texturePath
    );
    Drawable(
        const std::string& textureFrontPath,
        const std::string& textureLeftPath,
        const std::string& textureRightPath
    );
    Drawable(
        const std::string& texturePath,
        float width,
        float height
    );
    Drawable(
        const std::string& textureFrontPath,
        const std::string& textureLeftPath,
        const std::string& textureRightPath,
        float width,
        float height
    );
    ~Drawable();

    void Draw(float x, float y, int side = 1);
    void DrawDragging(float x, float y, int side = 1);
    void DrawCollisioning(float x, float y, int side = 1);
    void SetScale(float newScale) { scale = newScale; };
    float GetScale() const { return scale; };
    Vector2 GetSize() const { return {width * scale, height * scale}; };
    Rectangle GetBoundingBox(float x, float y) const;
    bool IsPointInside(float x, float y) const;
};

#endif