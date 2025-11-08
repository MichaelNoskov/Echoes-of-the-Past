#ifndef STATESDRAWABLE_H
#define STATESDRAWABLE_H

#include "raylib.h"
#include "TextureRenderer.h"
#include "Drawable.h"
#include <string>
#include <vector>
#include <map>

struct StatesDrawContext {
    Vector2 position;
    float scale = 1;
    int method = 0;
};

class StatesDrawable : public Drawable<StatesDrawContext> {
private:
    std::map<std::string, TextureRenderer> textures;
    std::string currentState = "default";

public:
    StatesDrawable(
        const std::map<std::string, TextureRenderer>& textures
    );

    StatesDrawable(
        TextureRenderer texture
    );

    void SetState(std::string newState);
    std::string GetState() { return currentState; };
    void Draw(const StatesDrawContext& context) override;
};

#endif