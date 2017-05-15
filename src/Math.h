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

inline Vector2 add(Vector2 a, Vector2 b)
{
    Vector2 result = {};
    
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    
    return result;
}

inline Vector2 scalarMultiply(Vector2 a, int i)
{
    Vector2 result = {};
    
    result.x = a.x * i;
    result.y = a.y * i;
    
    return result;
}

#define MATH_H
#endif

