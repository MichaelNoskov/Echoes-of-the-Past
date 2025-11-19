#ifndef TRANSFORMABLE2D_H
#define TRANSFORMABLE2D_H

#include "raylib.h"
#include "Spatial.h"
#include <string>

class Transformable2D : public Spatial<Vector2, Rectangle> {
private:
    float scale = 1.0f;
    Vector2 size;
    Vector2 position;

public:
    Transformable2D(
        float width,
        float height,
        float x,
        float y
    );

    Rectangle GetBoundingBox() const override;
    Vector2 GetPosition() const override;
    void SetPosition(const Vector2& newPosition) override;

    bool Intersects(const Spatial<Vector2, Rectangle>& other) const override;
    bool Intersects(const Rectangle& otherRect) const override;
    bool ContainsPoint(const Vector2& point) const override;
    bool ContainsPoint(float x, float y) const;

    Vector2 GetSize() const override;
    float GetScale() const;

    Vector2 Move(const Vector2& shift);
    void SetScale(float newScale);

    float ScaleToFit(float targetWidth, float targetHeight);
};

#endif