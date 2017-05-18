#ifndef BOARD_H

const uint8_t DIM = 7;


// TODO(jan): Move the player class to a smarter location
enum Player
{
    PLAYER_WHITE, PLAYER_BLACK
};

class GameState
{
    private:
    void copyFieldsTo(GameState* dest);
    bool isFieldPosValid(Vector2 pos);
    Field* getFieldAtPos(Vector2 pos);
	int16_t val = NULL;
    void calculateMovesInDirection(Player player, 
                                   uint8_t row, 
                                   uint8_t col, 
                                   Vector2 dir);
    void testCaptureInDirection(Player player, 
                                Vector2 testFieldP, 
                                Vector2 testDir);
    
    public:
    Field fields[DIM][DIM];
    GameState* firstChild = 0;
    GameState* nextSibling = 0;
    GameState* parent = 0;
    uint32_t childCount = 0;
    void draw();
	int16_t evaluate();
	void recEvaluate(int maxDepth);
    void calculateNextMoves(Player player);
	Vector2 kingPos = {};
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