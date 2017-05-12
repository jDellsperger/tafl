#include "Board.h"

void GameState::draw()
{
    for (int y = 0; y < DIM; y++)
    {
        for (int x = 0; x < DIM; x++)
        {
            std::string status;
            Field f = this->fields[y][x];
            
            if (f.hasFlags(TARGET))
            {
                status = "T";
            }
            else if (f.hasFlags(KING))
            {
                status = "K";
            }
            else if (f.hasFlags(WHITE))
            {
                status = "W";
            }
            else if (f.hasFlags(BLACK))
            {
                status = "B";
            }
            else if (f.hasFlags(THRONE))
            {
                status = "C";
            }
            else
            {
                status = "+";
            }
            
            std::cout << status;
        }
        
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
}

void GameState::calculateMovesInDirection(uint8_t row, uint8_t col, Vector2 dir)
{
    Vector2 end = {row, col};
    end.add(dir);
    
    Field testField = this->fields[end.y][end.x];
    while ((end.y >= 0) && 
           (end.y < DIM) &&
           (end.x >= 0) &&
           (end.x < DIM) &&
           (!testField.hasFlags(WHITE | BLACK | KING)))
    {
        if (!testField.hasFlags(BLOCKING))
        {
            Move* m = new Move();
            m->start = {col, row};
            m->end = end;
            
            m->resulting = new GameState();
            this->copyFieldsTo(m->resulting);
            m->resulting->fields[col][row].setFlags(0);
            m->resulting->fields[end.y][end.x].setFlags(BLACK);
            
            m->nextSibling = this->firstChild;
            this->firstChild = m;
            
            this->moveCount++;
            
            // TODO(jan): Check if an enemy token was captured
        }
        
        end.add(dir);
        testField = this->fields[end.y][end.x];
    }
}

void GameState::calculateNextBlackMoves()
{
    //NOTE(jan): Iterate over all fields to find black tokens
    for (uint8_t col = 0; col < DIM; col++)
    {
        for (uint8_t row = 0; row < DIM; row++)
        {
            Field f = this->fields[col][row];
            
            // NOTE(jan): If there is a black token, calculate moves
            // for free fields up, down, left and right
            if (f.hasFlags(BLACK))
            {
                // Test up
                Vector2 dir = {0, -1};
                this->calculateMovesInDirection(row, col, dir);
                
                // Test right
                dir = {1, 0};
                this->calculateMovesInDirection(row, col, dir);
                
                // Test down;
                dir = {0, 1};
                this->calculateMovesInDirection(row, col, dir);
                
                // Test left
                dir = {-1, 0};
                this->calculateMovesInDirection(row, col, dir);
            }
        }
    }
}

void Field::setFlags(uint8_t flags)
{
    this->flags = flags;
}

bool Field::hasFlags(uint8_t flags)
{
    bool result = this->flags & flags;
    
    return result;
}
