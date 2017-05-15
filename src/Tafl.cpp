#include <string>
#include <iostream>

#include "Math.h"
#include "Board.cpp"

void initBrandubh(Board* b)
{
    b->state = new GameState();
    
    b->state->fields[0][0].setFlags(TARGET | BLOCKING);
    b->state->fields[DIM-1][DIM-1].setFlags(TARGET | BLOCKING);
    b->state->fields[0][DIM-1].setFlags(TARGET | BLOCKING);
    b->state->fields[DIM-1][0].setFlags(TARGET | BLOCKING);
    
    b->state->fields[0][3].setFlags(BLACK);
    b->state->fields[1][3].setFlags(BLACK);
    b->state->fields[5][3].setFlags(BLACK);
    b->state->fields[6][3].setFlags(BLACK);
    b->state->fields[3][0].setFlags(BLACK);
    b->state->fields[3][1].setFlags(BLACK);
    b->state->fields[3][5].setFlags(BLACK);
    b->state->fields[3][6].setFlags(BLACK);
    
    b->state->fields[3][2].setFlags(WHITE);
    b->state->fields[3][4].setFlags(WHITE);
    b->state->fields[2][3].setFlags(WHITE);
    b->state->fields[4][3].setFlags(WHITE);
    b->state->fields[3][3].setFlags(KING | THRONE | BLOCKING);
}

void initTest(Board* b)
{
    b->state = new GameState();
    b->state->fields[3][3].setFlags(WHITE);
    b->state->fields[2][4].setFlags(BLACK);
    b->state->fields[4][3].setFlags(BLACK);
}

int main()
{
    Board b;
    initBrandubh(&b);
    //initTest(&b);
    
    Player blackPlayer = {BLACK, WHITE};
    Player whitePlayer = {WHITE, BLACK};
    
    b.state->draw();
    b.state->calculateNextMoves(&blackPlayer);
    
    std::cout << "Black Player Move count: ";
    std::cout << std::to_string(b.state->moveCount) << std::endl;
    
    Move* m = b.state->firstChild;
    while(m)
    {
        std::cout << "Start -> col: " << std::to_string(m->start.y);
        std::cout << " row: " << std::to_string(m->start.x) << std::endl;
        
        std::cout << "End -> col: " << std::to_string(m->end.y);
        std::cout << " row: " << std::to_string(m->end.x) << std::endl;
        
        m->resulting->draw();
        m = m->nextSibling;
    }
    
    std::cin.get();
    
    std::cout << std::endl << std::endl;
    std::cout << "White Player Round 1:" << std::endl;
    
    m = b.state->firstChild;
    
    m->resulting->draw();
    m->resulting->calculateNextMoves(&whitePlayer);
    
    std::cout << "White Player Move Count: ";
    std::cout << std::to_string(m->resulting->moveCount) << std::endl;
    
    m = m->resulting->firstChild;
    while (m)
    {
        std::cout << "Start -> col: " << std::to_string(m->start.y);
        std::cout << " row: " << std::to_string(m->start.x) << std::endl;
        
        std::cout << "End -> col: " << std::to_string(m->end.y);
        std::cout << " row: " << std::to_string(m->end.x) << std::endl;
        
        m->resulting->draw();
        m = m->nextSibling;
    }
    
    std::cin.get();
    
    return 0;
}
