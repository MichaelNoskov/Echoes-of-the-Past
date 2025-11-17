#include "Transformable2D.h"

Transformable2D::Transformable2D(
    float width,
    float height,
    float x,
    float y
) : size{width, height}, position{x, y} {}

Rectangle Transformable2D::GetBoundingBox() const {
    return Rectangle{
        position.x,
        position.y,
        size.x * scale,
        size.y * scale
    };
}

Vector2 Transformable2D::GetPosition() const {
    return position;
}

void Transformable2D::SetPosition(const Vector2& newPosition) {
    position = newPosition;
}

bool Transformable2D::Intersects(const Spatial<Vector2, Rectangle>& other) const {
    Rectangle thisRect = GetBoundingBox();
    Rectangle otherRect = other.GetBoundingBox();
    return CheckCollisionRecs(thisRect, otherRect);
}

bool Transformable2D::Intersects(const Rectangle& otherRect) const {
    Rectangle thisRect = GetBoundingBox();
    return CheckCollisionRecs(thisRect, otherRect);
}

bool Transformable2D::ContainsPoint(const Vector2& point) const {
    Rectangle thisRect = GetBoundingBox();
    return CheckCollisionPointRec(point, thisRect);
}

bool Transformable2D::ContainsPoint(float x, float y) const {
    return ContainsPoint(Vector2{x, y});
}

Vector2 Transformable2D::GetSize() const {
    return size;
}

float Transformable2D::GetScale() const {
    return scale;
}

Vector2 Transformable2D::Move(const Vector2& shift) {
    position.x += shift.x;
    position.y += shift.y;
    return position;
}

void Transformable2D::SetScale(float newScale) {
    scale = newScale;
}

float Transformable2D::ScaleToFit(float targetWidth, float targetHeight) {
    float scaleX = targetWidth / size.x;
    float scaleY = targetHeight / size.y;

    scale = (scaleX < scaleY) ? scaleX : scaleY;
    
    return scale;
}
