#ifndef BOARD_H

const uint8_t DIM = 7;

enum FieldFlag
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
    void removeFlags(uint8_t flags);
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

// TODO(jan): Move the player class to a smarter location
class Player
{
    public:
    FieldFlag allyFlag;
    FieldFlag enemyFlag;
};

class GameState
{
    private:
    void copyFieldsTo(GameState* dest);
    bool isFieldPosValid(Vector2 pos);
    Field* getFieldAtPos(Vector2 pos);
    void calculateMovesInDirection(Player* player, uint8_t row, 
                                   uint8_t col, Vector2 dir);
    void testCaptureInDirection(Player* player, Vector2 testFieldP, Vector2 testDir);
    
    public:
    Field fields[DIM][DIM];
    Move* firstChild = 0;
    uint32_t moveCount = 0;
    void draw();
    void calculateNextMoves(Player* player);
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

inline bool GameState::isFieldPosValid(Vector2 pos)
{
    bool result = 
        (pos.x >= 0) &&
        (pos.y >= 0) &&
        (pos.x < DIM) &&
        (pos.y < DIM);
    
    return result;
}

inline Field* GameState::getFieldAtPos(Vector2 pos)
{
    Field* result = &this->fields[pos.y][pos.x];
    
    return result;
}

class Board
{
    public:
    GameState* state;
};

#define BOARD_H
#endif