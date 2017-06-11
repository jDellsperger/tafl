#ifndef GAMESTATE_H

// TODO(jan): Move the player class to a smarter location
enum Player
{
    PLAYER_NONE = 0,
    PLAYER_MAX = 1,
    PLAYER_MIN = 2,
    PLAYER_BOTH = 3
};

enum FieldState : uint8_t
{
    FIELD_EMPTY = 0,
    FIELD_KING = 1,
    FIELD_WHITE = 2,
    FIELD_BLACK = 4
};

class Move;
class Board;

class GameState
{
    private:
    FieldState getStateAtPos(Vector2 pos);
    int16_t calcQuadrantValue(Vector2 kingPos, Vector2 tPos);
    void setStateAtPos(Vector2 pos, FieldState s, Board* b);
    void addMinPlayerMove(GameState* resulting);
    void addMaxPlayerMove(GameState* resulting);
    void testCaptureAtPos(Vector2 pos, uint8_t allyState, 
                          FieldState enemyState, Board* b);
    
    public:
    uint16_t zobristHash;
    FieldState fields[DIM][DIM];
    int16_t val = INT16_MAX;
    Move* firstMaxPlayerMove = nullptr;
    Move* firstMinPlayerMove = nullptr;
    uint32_t maxPlayerMoveCount = 0;
    uint32_t minPlayerMoveCount = 0;
    Vector2 kingPos = {};
    bool maxPlayerMovesCalculated = false;
    bool minPlayerMovesCalculated = false;
    uint8_t repetitionCount = 0;
    std::string info;
    
    void generateZobristHash();
    void draw();
    void minimax(int cutoff, Player player);
    void abPruning(int cutOff, Player player, int alpha, int beta);
    int16_t evaluate();
    void calculateNextMaxPlayerMoves();
    void calculateNextMinPlayerMoves();
    Player checkVictory();
    bool equals(GameState* s);
    void copyFieldsTo(GameState* dest);
    bool hasStateAtPos(Vector2 pos, FieldState s);
    int calcHops();
};

class GraphVertex
{
    public:
    GraphVertex* edges[2*(DIM-1)];
    int edgeCount = 0;
    int index;
};

class Move
{
    public:
    GameState* resulting;
    Move* nextAlternative;
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

inline bool GameState::equals(GameState* s)
{
    for (uint8_t col = 0; col < DIM; col++)
    {
        for (uint8_t row = 0; row < DIM; row++)
        {
            if (s->fields[col][row] != this->fields[col][row])
            {
                return false;
            }
        }
    }
    
    return true;
}

inline FieldState GameState::getStateAtPos(Vector2 pos)
{
    FieldState result = this->fields[pos.y][pos.x];
    
    return result;
}

inline bool GameState::hasStateAtPos(Vector2 pos, FieldState s)
{
    bool result = (this->fields[pos.y][pos.x] == s);
    
    return result;
}

#define GAMESTATE_H
#endif