#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "DrawContext.h"

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void Draw(const DrawContext& context) = 0;
};

#endif
