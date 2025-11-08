#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "raylib.h"
#include <string>
#include <vector>
#include <map>

class Drawable {
private:
    Texture2D texture;
    float width;
    float height;
    Vector2 offset;

    void DrawBottom(Vector2 position, float scale = 1);
    void DrawCenter(Vector2 position, float scale = 1);
    void DrawTop(Vector2 position, float scale = 1);

public:
    Drawable(
        const std::string& texturePath,
        float width,
        float height,
        Vector2 offset = {0, 0}
    );

    void Draw(Vector2 position, float scale = 1, int method = 0) {
        if (method == 0) {
            return DrawBottom(position, scale);
        }
        if (method == 1) {
            return DrawCenter(position, scale);
        }
        if (method == 2) {
            return DrawTop(position, scale);
        }
    };
};

#endif