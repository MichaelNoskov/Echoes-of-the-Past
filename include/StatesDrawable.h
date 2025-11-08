#ifndef STATESDRAWABLE_H
#define STATESDRAWABLE_H

#include "raylib.h"
#include <TextureRenderer.h>
#include <string>
#include <vector>
#include <map>

class StatesDrawable {
private:
    std::map<std::string, TextureRenderer> textures;
    std::string currentState = "default";

public:
    StatesDrawable(
        const std::map<std::string, TextureRenderer>& textures
    );

    StatesDrawable(
        TextureRenderer
    );

    void SetState(std::string newState);
    std::string GetState() { return currentState; };
    void Draw(Vector2 position, float scale, int method);
};

#endif