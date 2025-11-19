#ifndef STATESDRAWABLE_H
#define STATESDRAWABLE_H

#include "Drawable.h"
#include <string>
#include <map>
#include <memory>

class StatesDrawable : public Drawable {
private:
    std::map<std::string, std::shared_ptr<Drawable>> drawables;
    std::string currentState = "default";

public:
    StatesDrawable(const std::map<std::string, std::shared_ptr<Drawable>>& drawables) 
        : drawables(drawables) {
        if (drawables.find("default") == drawables.end() && !drawables.empty()) {
            currentState = drawables.begin()->first;
        }
    }

    StatesDrawable(std::shared_ptr<Drawable> drawable) {
        drawables["default"] = drawable;
        currentState = "default";
    }

    void SetState(const std::string& newState) {
        if (drawables.find(newState) != drawables.end()) {
            currentState = newState;
        }
    }

    std::string GetState() const { return currentState; }

    void Draw(const DrawContext& context) override {
        if (auto it = drawables.find(currentState); it != drawables.end()) {
            it->second->Draw(context);
        }
    }
};

#endif
