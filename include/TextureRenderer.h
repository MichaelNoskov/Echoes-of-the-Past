#ifndef TEXTURERENDERER_H
#define TEXTURERENDERER_H

#include "raylib.h"
#include "Drawable.h"
#include <string>

struct TextureDrawContext {
    Vector2 position;
    Vector2 size;
    int method = 0;
};

class TextureRenderer : public Drawable<TextureDrawContext> {
private:
    Texture2D texture;
    Vector2 offset;

    void DrawBottom(Vector2 position, Vector2 size);
    void DrawCenter(Vector2 position, Vector2 size);
    void DrawTop(Vector2 position, Vector2 size);

public:
    TextureRenderer(
        const std::string& texturePath,
        Vector2 offset = {0, 0}
    );

    void Draw(const TextureDrawContext& context) override {
        if (context.method == 0) {
            DrawBottom(context.position, context.size);
        } else if (context.method == 1) {
            DrawCenter(context.position, context.size);
        } else if (context.method == 2) {
            DrawTop(context.position, context.size);
        }
    }
};

#endif