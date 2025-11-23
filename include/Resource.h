#ifndef RESOURCE_H
#define RESOURCE_H

#include "raylib.h"
#include <string>

struct Resource {
    virtual ~Resource() = default;

    virtual std::string GetName() const = 0;
    virtual void Draw(const Rectangle& area) const = 0;
    virtual float GetValue() const = 0;
    virtual void SetValue(float newValue) = 0;
    virtual void Add(float value) = 0;
    virtual void Subtract(float value) = 0;
    virtual std::string GetDisplayText() const = 0;
};

#endif
