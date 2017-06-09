#include "Board.h"

Vector2 Board::thronePos = {(int)(DIM/2), (int)(DIM/2)};

GameState* Board::getGameState(GameState* s)
{
    GameState* result = nullptr;
    ZobristEntryNode e = this->zobristHashTable[s->zobristHash];
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

GameState* Board::addGameState(GameState* s)
{
    ZobristNode* n = new ZobristNode();
    
    s->copyFieldsTo(&n->state);
    n->state.kingPos = s->kingPos;
    n->state.zobristHash = s->zobristHash;
    n->state.info = "Added in round " + std::to_string(this->roundCount) + ".";
    
    ZobristEntryNode e = this->zobristHashTable[s->zobristHash];
    ZobristNode* n2 = e.firstNode;
    n->next = n2;
    e.firstNode = n;
    
    GameState* result = &n->state;
    
    return result;
}

GameState* Board::getZobristAddress(GameState* s)
{
    GameState* result = this->getGameState(s);
    
    if (!result)
    {
        result = this->addGameState(s);
    }
    
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


