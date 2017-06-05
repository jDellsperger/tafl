#ifndef MATH_H

class Vector2
{
    public:
    int16_t x, y;
    void add(Vector2 a);
    bool equals(Vector2 a);
};

inline void Vector2::add(Vector2 a)
{
    this->x += a.x;
    this->y += a.y;
}

inline bool Vector2::equals(Vector2 a)
{
    bool result =
        (this->x == a.x) &&
        (this->y == a.y);
    
    return result;
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

inline uint32_t distanceSq(Vector2 a, Vector2 b)
{
    uint32_t d = (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
    
    return d;
}

inline void draw(Vector2 a)
{
    std::cout << "X: " << a.x << " | " << "Y: " << a.y << std::endl;
}

inline int min(int a, int b) 
{
    int result = (a < b ? a : b);
    
    return result;
}

inline int max(int a, int b)
{
    int result = (a > b ? a : b);
    
    return result;
}

#define MATH_H
#endif

