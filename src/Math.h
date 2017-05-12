#ifndef MATH_H

class Vector2
{
    public:
    int16_t x, y;
    void add(Vector2 a);
};

inline void Vector2::add(Vector2 a)
{
    this->x += a.x;
    this->y += a.y;
}

#define MATH_H
#endif

