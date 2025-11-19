#include "TextureRenderer.h"
#include <raylib.h>

TextureRenderer::TextureRenderer(const std::string& texturePath, Vector2 offset) 
    : offset(offset) {
    texture = LoadTexture(texturePath.c_str());
}

TextureRenderer::~TextureRenderer() {
    UnloadTexture(texture);
}

void TextureRenderer::DrawBottom(const DrawContext& context) {
    Rectangle destRect = {
        context.position.x + offset.x,
        context.position.y + offset.y - context.size.y,
        context.size.x,
        context.size.y
    };
    Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, sourceRect, destRect, Vector2{0, 0}, 0, WHITE);
}

void TextureRenderer::DrawCenter(const DrawContext& context) {
    Rectangle destRect = {
        context.position.x + offset.x - context.size.x / 2,
        context.position.y + offset.y - context.size.y / 2,
        context.size.x,
        context.size.y
    };
    Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, sourceRect, destRect, Vector2{0, 0}, 0, WHITE);
}

void TextureRenderer::DrawTop(const DrawContext& context) {
    Rectangle destRect = {
        context.position.x + offset.x,
        context.position.y + offset.y,
        context.size.x,
        context.size.y
    };
    Rectangle sourceRect = {0, 0, (float)texture.width, (float)texture.height};
    DrawTexturePro(texture, sourceRect, destRect, Vector2{0, 0}, 0, WHITE);
}
