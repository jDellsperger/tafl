#ifndef BOARD_H

const uint8_t DIM = 7;

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

class GameState;

class Move
{
    public:
    Move* nextSibling = 0;
    GameState* resulting;
    Vector2 start;
    Vector2 end;
};

class GameState
{
    private:
    void copyFieldsTo(GameState* dest);
    void calculateMovesInDirection(uint8_t row, uint8_t col, Vector2 dir);
    
    public:
    Field fields[DIM][DIM];
    Move* firstChild = 0;
    uint32_t moveCount = 0;
    void draw();
    void calculateNextBlackMoves();
};

inline void GameState::copyFieldsTo(GameState* dest)
{
    for (uint8_t col = 0; col < DIM; col++)
    {
        for (uint8_t row = 0; row < DIM; row++)
        {
            dest->fields[col][row] = this->fields[col][row];
        }
    }
}

class Board
{
    public:
    GameState* state;
};

#define BOARD_H
#endif