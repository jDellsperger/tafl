#include <iostream>
#include "tafl.h"

void drawTafl(Tafl tafl)
{
    for (int y = 0; y < DIM; y++)
    {
        for (int x = 0; x < DIM; x++)
        {
            std::string status;
            Field f = tafl.fields[y][x];
            
            if (f.flags & TARGET)
            {
                status = "T";
            }
            else if (f.flags & CENTER)
            {
                status = "C";
            }
            else if (f.flags & KING)
            {
                status = "K";
            }
            else if (f.flags & WHITE)
            {
                status = "W";
            }
            else if (f.flags & BLACK)
            {
                status = "B";
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

int main()
{
    Tafl t;
    t.fields[0][0].flags = TARGET;
    t.fields[DIM-1][DIM-1].flags = TARGET;
    t.fields[0][DIM-1].flags = TARGET;
    t.fields[DIM-1][0].flags = TARGET;
    
    t.fields[0][3].flags = BLACK;
    t.fields[1][3].flags = BLACK;
    t.fields[5][3].flags = BLACK;
    t.fields[6][3].flags = BLACK;
    t.fields[3][0].flags = BLACK;
    t.fields[3][1].flags = BLACK;
    t.fields[3][5].flags = BLACK;
    t.fields[3][6].flags = BLACK;
    
    t.fields[3][2].flags = WHITE;
    t.fields[3][4].flags = WHITE;
    t.fields[2][3].flags = WHITE;
    t.fields[4][3].flags = WHITE;
    
    t.fields[3][3].flags = KING;
    
    drawTafl(t);
    
    return 0;
}