#ifndef RESOURCE_H
#define RESOURCE_H

#include "raylib.h"
#include <string>

struct Resource {
    virtual ~Resource() = default;

    virtual void Draw(const Rectangle& area) const = 0;

    virtual int GetValue() const = 0;

    virtual void SetValue(int newValue) = 0;

    virtual std::string GetDisplayText() const = 0;
};

#endif