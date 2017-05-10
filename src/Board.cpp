#include <iostream>
#include "../inc/Board.h"

void Board::draw()
{
    GameState state = *this->state;
    for (int y = 0; y < DIM; y++)
    {
        for (int x = 0; x < DIM; x++)
        {
            std::string status;
            Field f = state.fields[y][x];
            
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
