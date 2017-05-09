#ifndef HNEFATAFL_H

enum FieldFlags
{
TARGET = 1,
CENTER = 2,
KING = 4,
WHITE = 8,
BLACK = 16,
};

struct Field
{
    uint8_t flags = 0;
};

const uint8_t DIM = 7;

struct Tafl
{
    Field fields[DIM][DIM];
};

#define HNEFATAFL_H
#endif