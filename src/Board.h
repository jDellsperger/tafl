#ifndef BOARD_H

const uint8_t DIM = 7;

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
    GameState* getGameState(uint16_t hash, GameState* s);
    GameState* addGameState(uint16_t hash, GameState* s);
};

Vector2 Board::thronePos = {(int)(DIM/2), (int)(DIM/2)};

inline bool Board::isFieldPosValid(Vector2 pos)
{
    bool result = 
        (pos.x >= 0) &&
        (pos.y >= 0) &&
        (pos.x < DIM) &&
        (pos.y < DIM);
    
    return result;
}

GameState* Board::getGameState(uint16_t hash, GameState* s)
{
    GameState* result = nullptr;
    ZobristEntryNode e = this->zobristHashTable[hash];
    ZobristNode* n = e.firstNode;
    
    while (n != nullptr) {
        if (n->state.equals(s))
        {
            result = &n->state;
            break;
        }
        
        n = n->next;
    };
    
    return result;
}

GameState* Board::addGameState(uint16_t hash, GameState* s)
{
    ZobristNode* n = new ZobristNode();
    
    s->copyFieldsTo(&n->state);
    n->state.kingPos = s->kingPos;
    n->state.zobristHash = hash;
    n->state.info = "Added in round " + std::to_string(this->roundCount) + ".";
    
    ZobristEntryNode e = this->zobristHashTable[hash];
    ZobristNode* n2 = e.firstNode;
    n->next = n2;
    e.firstNode = n;
    
    GameState* result = &n->state;
    
    return result;
}

Board::Board()
{
    this->state = new GameState();
    
    for (int i = 0; i < DIM; i++)
    {
        for (int j = 0; j < DIM; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                this->zobristValues[i][j][k] = rand();
            }
        }
    }
}

Board* Board::getInstance()
{
    
    static Board* instance = new Board();
    
    return instance;
}

inline uint16_t Board::getZobristValue(Vector2 pos, uint8_t s)
{
    int result = 0;
    
    if (s)
    {
        int stateIndex;
        if (s == FIELD_KING)
        {
            stateIndex = 0;
        }
        else if (s == FIELD_WHITE)
        {
            stateIndex = 1;
        }
        else if (s == FIELD_BLACK)
        {
            stateIndex = 2;
        }
        
        result = this->zobristValues[pos.y][pos.x][stateIndex];
    }
    
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

