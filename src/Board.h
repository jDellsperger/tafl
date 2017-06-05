#ifndef BOARD_H

const uint8_t DIM = 7;

#include "Gamestate.h"

class Board
{
    private:
    static Vector2 thronePos;
    int zobristValues[DIM][DIM][3];
    Board();
    
    public:
    GameState* state;
    
    int getZobristValue(Vector2 pos, uint8_t s);
    
    static Board* getInstance();
    static bool isFieldPosTarget(Vector2 pos);
    static bool isFieldPosThrone(Vector2 pos);
};

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

inline int Board::getZobristValue(Vector2 pos, uint8_t s)
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
    
    int result = this->zobristValues[pos.y][pos.x][stateIndex];
    
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

