#include "Board.cpp"

int main()
{
     Board b;
    b.state = new GameState();
    
    b.state->fields[0][0].setFlags(TARGET | BLOCKING);
    b.state->fields[DIM-1][DIM-1].setFlags(TARGET | BLOCKING);
    b.state->fields[0][DIM-1].setFlags(TARGET | BLOCKING);
    b.state->fields[DIM-1][0].setFlags(TARGET | BLOCKING);
    
    b.state->fields[0][3].setFlags(BLACK);
    b.state->fields[1][3].setFlags(BLACK);
    b.state->fields[5][3].setFlags(BLACK);
    b.state->fields[6][3].setFlags(BLACK);
    b.state->fields[3][0].setFlags(BLACK);
    b.state->fields[3][1].setFlags(BLACK);
    b.state->fields[3][5].setFlags(BLACK);
    b.state->fields[3][6].setFlags(BLACK);
    
    b.state->fields[3][2].setFlags(WHITE);
    b.state->fields[3][4].setFlags(WHITE);
    b.state->fields[2][3].setFlags(WHITE);
    b.state->fields[4][3].setFlags(WHITE);
    
    b.state->fields[3][3].setFlags(KING | THRONE | BLOCKING);
    
    b.draw();
    
    getchar();
    
    return 0;
}