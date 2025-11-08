#ifndef DRAWABLE_H
#define DRAWABLE_H

template<typename Context>
class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void Draw(const Context& context) = 0;
};

#endif