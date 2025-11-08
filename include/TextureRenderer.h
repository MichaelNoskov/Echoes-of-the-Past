#ifndef TEXTURERENDERER_H
#define TEXTURERENDERER_H

#include "raylib.h"
#include "Drawable.h"
#include <string>

struct TextureDrawContext {
    Vector2 position;
    float scale;
    int method;
};

class TextureRenderer : public Drawable<TextureDrawContext> {
private:
    Texture2D texture;
    float width;
    float height;
    Vector2 offset;

    void DrawBottom(Vector2 position, float scale = 1);
    void DrawCenter(Vector2 position, float scale = 1);
    void DrawTop(Vector2 position, float scale = 1);

public:
    TextureRenderer(
        const std::string& texturePath,
        float width,
        float height,
        Vector2 offset = {0, 0}
    );

    void Draw(const TextureDrawContext& context) override {
        if (context.method == 0) {
            DrawBottom(context.position, context.scale);
        } else if (context.method == 1) {
            DrawCenter(context.position, context.scale);
        } else if (context.method == 2) {
            DrawTop(context.position, context.scale);
        }
    }
};

#endif