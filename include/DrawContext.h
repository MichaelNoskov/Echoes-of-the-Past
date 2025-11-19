#ifndef DRAWCONTEXT_H
#define DRAWCONTEXT_H

#include "raylib.h"

struct DrawContext {
    Vector2 position;
    Vector2 size;
    int alignment = 0;
    
    DrawContext() = default;
    DrawContext(Vector2 pos, Vector2 sz, int align = 0) 
        : position(pos), size(sz), alignment(align) {}
};

#endif