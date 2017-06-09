#ifndef BOARD_H

const uint8_t DIM = 7;
const Vector2 DIRECTIONS[4] = {
    {0, 1},
    {0, -1},
    {1, 0},
    {-1, 0}
};

#include "Gamestate.h"

class ZobristNode
{
    public:
    ZobristNode* next = nullptr;
    GameState state;
};

class ZobristEntryNode
{
    public:
    ZobristNode* firstNode = nullptr;
};

class Board
{
    private:
    static Vector2 thronePos;
    
    Board();
    GameState* getGameState(GameState* s);
    GameState* addGameState(GameState* s);
    
    public:
    GameState* state;
    // TODO(jan): is this the best place for this? 
    // how large should the hash table be?
    ZobristEntryNode zobristHashTable[UINT16_MAX + 1];
    uint16_t zobristValues[DIM][DIM][3];
    int roundCount = 0;
    
    static Board* getInstance();
    static bool isFieldPosTarget(Vector2 pos);
    static bool isFieldPosThrone(Vector2 pos);
    static bool isFieldPosValid(Vector2 pos);
    uint16_t getZobristValue(Vector2 pos, uint8_t s);
    GameState* getZobristAddress(GameState* s);
};

inline bool Board::isFieldPosValid(Vector2 pos)
{
    bool result = 
        (pos.x >= 0) &&
        (pos.y >= 0) &&
        (pos.x < DIM) &&
        (pos.y < DIM);
    
    return result;
}

inline bool Board::isFieldPosTarget(Vector2 pos)
{
    bool result = false;
    
    // TODO(jan): extract target vectors into array in board class
    if (pos.x == 0)
    {
        if (pos.y == 0 || pos.y == DIM - 1)
        {
            result = true;
        }
    }
    else if (pos.x == DIM - 1)
    {
        if (pos.y == 0 || pos.y == DIM - 1)
        {
            result = true;
        }
    }
    
    return result;
}

inline bool Board::isFieldPosThrone(Vector2 pos)
{
    bool result = Board::thronePos.equals(pos);
    
    return result;
}

#define BOARD_H
#endif

