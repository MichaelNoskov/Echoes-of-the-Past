#ifndef SPATIAL_H
#define SPATIAL_H

#include "raylib.h"

template<typename Context>
class Spatial {
public:
    virtual ~Spatial() = default;

    virtual auto GetBoundingBox() const = 0;
    virtual Context GetPosition() const = 0;
    virtual void SetPosition(const Context& pos) = 0;
};

#endif
