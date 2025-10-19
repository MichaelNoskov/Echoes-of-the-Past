#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "raylib.h"
#include <string>

class Drawable {
private:
    float width;
    float height;
    Texture2D texture;

public:
    Drawable(const std::string& texturePath, float width, float height);
    Drawable(const std::string& texturePath);
    ~Drawable();

    void Draw(float x, float y);
};

#endif