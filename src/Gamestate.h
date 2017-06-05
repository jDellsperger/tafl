#ifndef BOARD_H

const uint8_t DIM = 7;

// TODO(jan): Move the player class to a smarter location
enum Player
{
    PLAYER_NONE = 0,
    PLAYER_MAX = 1,
    PLAYER_MIN = 2
};

class GameState
{
    private:
    void copyFieldsTo(GameState* dest);
    bool isFieldPosValid(Vector2 pos);
    Field* getFieldAtPos(Vector2 pos);
    void calculateMovesInDirection(Player player, 
                                   uint8_t row, 
                                   uint8_t col, 
                                   Vector2 dir);
    void testCaptureInDirection(Player player, 
                                Vector2 testFieldP, 
                                Vector2 testDir);
    int16_t calcQuadrantValue(Vector2 kingPos, Vector2 tPos);
    
    public:
    Field fields[DIM][DIM];
    int16_t val = INT16_MAX;
    GameState* firstChild = nullptr;
    GameState* nextSibling = nullptr;
    GameState* parent = nullptr;
    uint32_t childCount = 0;
    void draw();
    void minimax(int cutoff, Player player);
    int16_t evaluate();
    void calculateNextMoves(Player player);
    Vector2 kingPos = {};
    Player checkVictory();
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
    private:
    static Vector2 thronePos;
    
    public:
    GameState* state;
    static bool isFieldPosTarget(Vector2 pos);
    static bool isFieldPosThrone(Vector2 pos);
};

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