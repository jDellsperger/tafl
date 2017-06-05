#ifndef FIELD_H

enum FieldState
{
    FIELD_EMPTY = 0,
    FIELD_KING = 1,
    FIELD_WHITE = 2,
    FIELD_BLACK = 4
};

class Field
{
    public:
    uint8_t flags = FIELD_EMPTY;
    void setFlags(uint8_t flags);
    void removeFlags(uint8_t flags);
    bool hasOneOfFlags(uint8_t flags);
    bool hasState(FieldState s);
};

inline void Field::setFlags(uint8_t flags)
{
    this->flags = (this->flags | flags);
}

inline void Field::removeFlags(uint8_t flags)
{
    this->flags = (this->flags & ~flags);
}

// NOTE(jan): Checks if the field has ONE of the flags passed OR-ed together
inline bool Field::hasOneOfFlags(uint8_t flags)
{
    bool result = this->flags & flags;
    
    return result;
}

// NOTE(jan): Checks if the fields flags match the passed state exactly
inline bool Field::hasState(FieldState s)
{
    bool result = (this->flags == s);
    
    return result;
}

#define FIELD_H
#endif



