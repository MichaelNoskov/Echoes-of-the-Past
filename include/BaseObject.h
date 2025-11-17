#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

#include "Spatial.h"
#include "Drawable.h"
#include <memory>

template<typename Context, typename BoundingType>
class BaseObject {
private:
    std::shared_ptr<Spatial<Context, BoundingType>> spatial;
    std::shared_ptr<Drawable<Context>> drawable;

public:
    BaseObject(
        std::shared_ptr<Spatial<Context, BoundingType>> spatialObj = nullptr,
        std::shared_ptr<Drawable<Context>> drawableObj = nullptr
    ) : spatial(spatialObj), drawable(drawableObj) {}

    BoundingType GetBoundingBox() const {
        return spatial ? spatial->GetBoundingBox() : BoundingType{};
    }

    Context GetPosition() const {
        return spatial ? spatial->GetPosition() : Context{};
    }

    void SetPosition(const Context& pos) {
        if (spatial) spatial->SetPosition(pos);
    }

    bool Intersects(const Spatial<Context, BoundingType>& other) const {
        return spatial ? spatial->Intersects(other) : false;
    }

    bool Intersects(const BoundingType& otherBounds) const {
        return spatial ? spatial->Intersects(otherBounds) : false;
    }

    bool ContainsPoint(const Context& point) const {
        return spatial ? spatial->ContainsPoint(point) : false;
    }

    void Draw(const Context& context) {
        if (drawable) drawable->Draw(context);
    }

    void SetSpatial(std::shared_ptr<Spatial<Context, BoundingType>> newSpatial) {
        spatial = newSpatial;
    }

    void SetDrawable(std::shared_ptr<Drawable<Context>> newDrawable) {
        drawable = newDrawable;
    }

    std::shared_ptr<Spatial<Context, BoundingType>> GetSpatial() const {
        return spatial;
    }

    std::shared_ptr<Drawable<Context>> GetDrawable() const {
        return drawable;
    }

    bool HasSpatial() const { return spatial != nullptr; }
    bool HasDrawable() const { return drawable != nullptr; }
    bool IsValid() const { return spatial != nullptr && drawable != nullptr; }
};

#endif