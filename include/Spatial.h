#ifndef SPATIAL_H
#define SPATIAL_H

#include "raylib.h"

template<typename Context, typename BoundingType>
class Spatial {
public:
    virtual ~Spatial() = default;

    virtual BoundingType GetBoundingBox() const = 0;
    virtual Context GetPosition() const = 0;
    virtual void SetPosition(const Context& pos) = 0;

    virtual bool Intersects(const Spatial<Context, BoundingType>& other) const = 0;
    virtual bool Intersects(const BoundingType& otherBounds) const = 0;
    virtual bool ContainsPoint(const Context& point) const = 0;
};

#endif