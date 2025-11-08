#include <TextureRenderer.h>
#include <raylib.h>

TextureRenderer::TextureRenderer(
    const std::string& texturePath,
    float width,
    float height,
    Vector2 offset
) : width(width), height(height), offset(offset) {
    texture = LoadTexture(texturePath.c_str());
}

TextureRenderer::~TextureRenderer() {
    UnloadTexture(texture);
}

void TextureRenderer::DrawBottom(Vector2 position, float scale) {
    Vector2 drawPos = {
        position.x + offset.x,
        position.y + offset.y - (height * scale)
    };
    DrawTextureEx(texture, drawPos, 0, scale, WHITE);
}

void TextureRenderer::DrawCenter(Vector2 position, float scale) {
    Vector2 drawPos = {
        position.x + offset.x - (width * scale) / 2,
        position.y + offset.y - (height * scale) / 2
    };
    DrawTextureEx(texture, drawPos, 0, scale, WHITE);
}

void TextureRenderer::DrawTop(Vector2 position, float scale) {
    Vector2 drawPos = {
        position.x + offset.x,
        position.y + offset.y
    };
    DrawTextureEx(texture, drawPos, 0, scale, WHITE);
}
