#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "raylib.h"
#include <string>

class Drawable {
private:
    float width;
    float height;
    float scale = 1.0f;
    Texture2D textureFront;
    Texture2D textureRight;
    Texture2D textureLeft;

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

    void Draw(float x, float y);
    void SetScale(float newScale) { scale = newScale; };
    float GetScale() const { return scale; };
    Rectangle GetBoundingBox(float x, float y) const;
    bool IsPointInside(float x, float y) const;
};

#endif