#ifndef TRANSFORMABLE2D_H
#define TRANSFORMABLE2D_H

#include "raylib.h"
#include "Spatial.h"
#include <string>

class Transformable2D : public Spatial<Vector2> {
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

    Vector2 GetSize() const;
    float GetScale() const;

    Vector2 Move(const Vector2& shift);
    void SetScale(float newScale);

    float ScaleToFit(float targetWidth, float targetHeight);
};

#endif