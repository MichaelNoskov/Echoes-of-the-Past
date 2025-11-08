#include "StatesDrawable.h"

StatesDrawable::StatesDrawable(
    const std::map<std::string, TextureRenderer>& textures
) : textures(textures) {
    if (textures.find("default") == textures.end() && !textures.empty()) {
        currentState = textures.begin()->first;
    }
}

StatesDrawable::StatesDrawable(TextureRenderer texture) {
    textures["default"] = texture;
    currentState = "default";
}

void StatesDrawable::SetState(std::string newState) {
    if (textures.find(newState) != textures.end()) {
        currentState = newState;
    }
}

void StatesDrawable::Draw(const StatesDrawContext& context) {
    TextureDrawContext texContext;
    texContext.position = context.position;
    texContext.scale = context.scale;
    texContext.method = context.method;
    textures[currentState].Draw(texContext);
}
