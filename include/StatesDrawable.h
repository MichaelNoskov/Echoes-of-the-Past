#ifndef STATESDRAWABLE_H
#define STATESDRAWABLE_H

#include "raylib.h"
#include "Drawable.h"
#include <string>
#include <map>
#include <memory>

struct StatesDrawContext {
    Vector2 position;
    float scale = 1;
    int method = 0;
};

template<typename Context>
class StatesDrawable : public Drawable<Context> {
private:
    std::map<std::string, std::shared_ptr<Drawable<Context>>> drawables;
    std::string currentState = "default";

public:
    StatesDrawable(
        const std::map<std::string, std::shared_ptr<Drawable<Context>>>& drawables
    ) : drawables(drawables) {
        if (drawables.find("default") == drawables.end() && !drawables.empty()) {
            currentState = drawables.begin()->first;
        }
    }

    StatesDrawable(
        std::shared_ptr<Drawable<Context>> drawable
    ) {
        drawables["default"] = drawable;
        currentState = "default";
    }

    void SetState(std::string newState) {
        if (drawables.find(newState) != drawables.end()) {
            currentState = newState;
        }
    }

    std::string GetState() { return currentState; }

    void Draw(const Context& context) override {
        if (auto it = drawables.find(currentState); it != drawables.end()) {
            it->second->Draw(context);
        }
    }
};

#endif
