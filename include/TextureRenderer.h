#ifndef TEXTURERENDERER_H
#define TEXTURERENDERER_H

#include "raylib.h"
#include "Drawable.h"
#include <string>

struct TextureDrawContext {
    Vector2 position;
    float scale = 1;
    int method = 0;
};

class TextureRenderer : public Drawable<TextureDrawContext> {
private:
    Texture2D texture;
    float width;
    float height;
    Vector2 offset;

    void DrawBottom(Vector2 position, float scale);
    void DrawCenter(Vector2 position, float scale);
    void DrawTop(Vector2 position, float scale);

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