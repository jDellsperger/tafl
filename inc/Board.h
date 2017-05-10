#ifndef BOARD_H

enum FieldFlags
{
TARGET = 1,
 THRONE = 2,
KING = 4,
WHITE = 8,
BLACK = 16,
    BLOCKING = 32
};

 class Field
{
    public:
    uint8_t flags = 0;
    void setFlags(uint8_t flags);
    bool hasFlags(uint8_t flags);
};

const uint8_t DIM = 7;

 class GameState;

 class Move
{
    public:
    Move* next;
    GameState* resulting;
};

 class GameState
{
    public:
    Field fields[DIM][DIM];
    Move* firstMove;
};

 class Board
{
    public:
    GameState* state;
    void draw();
};

#define BOARD_H
#endif