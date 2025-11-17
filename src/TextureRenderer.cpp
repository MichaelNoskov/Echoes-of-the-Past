#include "TextureRenderer.h"
#include <raylib.h>

TextureRenderer::TextureRenderer(
    const std::string& texturePath,
    Vector2 offset
) : offset(offset) {
    texture = LoadTexture(texturePath.c_str());
}

TextureRenderer::~TextureRenderer() {
    UnloadTexture(texture);
}

void TextureRenderer::DrawBottom(Vector2 position, Vector2 size) {
    Rectangle destRect = {
        position.x + offset.x,
        position.y + offset.y - size.y,
        size.x,
        size.y
    };
    Rectangle sourceRect = {
        0, 0, 
        (float)texture.width, 
        (float)texture.height
    };
    DrawTexturePro(texture, sourceRect, destRect, Vector2{0, 0}, 0, WHITE);
}

void TextureRenderer::DrawCenter(Vector2 position, Vector2 size) {
    Rectangle destRect = {
        position.x + offset.x - size.x / 2,
        position.y + offset.y - size.y / 2,
        size.x,
        size.y
    };
    Rectangle sourceRect = {
        0, 0, 
        (float)texture.width, 
        (float)texture.height
    };
    DrawTexturePro(texture, sourceRect, destRect, Vector2{0, 0}, 0, WHITE);
}

void TextureRenderer::DrawTop(Vector2 position, Vector2 size) {
    Rectangle destRect = {
        position.x + offset.x,
        position.y + offset.y,
        size.x,
        size.y
    };
    Rectangle sourceRect = {
        0, 0, 
        (float)texture.width, 
        (float)texture.height
    };
    DrawTexturePro(texture, sourceRect, destRect, Vector2{0, 0}, 0, WHITE);
}
