#ifndef TEXTURERENDERER_H
#define TEXTURERENDERER_H

#include "Drawable.h"
#include <string>

class TextureRenderer : public Drawable {
private:
    Texture2D texture;
    Vector2 offset;

    void DrawBottom(const DrawContext& context);
    void DrawCenter(const DrawContext& context);
    void DrawTop(const DrawContext& context);

public:
    TextureRenderer(const std::string& texturePath, Vector2 offset = {0, 0});
    ~TextureRenderer();

    void Draw(const DrawContext& context) override {
        if (context.alignment == 0) {
            DrawBottom(context);
        } else if (context.alignment == 1) {
            DrawCenter(context);
        } else if (context.alignment == 2) {
            DrawTop(context);
        }
    }
};

#endif
