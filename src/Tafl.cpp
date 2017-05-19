#include <string>
#include <iostream>

#include "Math.h"
#include "Field.cpp"
#include "Gamestate.cpp"

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
    
    b->state->kingPos = {3, 3};
}

void initTest(Board* b)
{
    b->state = new GameState();
    b->state->fields[1][1].setFlags(BLACK);
    b->state->fields[1][2].setFlags(BLACK);
    b->state->fields[2][1].setFlags(BLACK);
    
    b->state->fields[1][5].setFlags(BLACK);
    b->state->fields[2][5].setFlags(BLACK);
    
    b->state->fields[5][5].setFlags(BLACK);
    b->state->fields[6][5].setFlags(BLACK);
    
    b->state->fields[3][3].setFlags(KING);
    
    b->state->kingPos = {3, 3};
}

int main()
{
    Board b;
    //initBrandubh(&b);
    initTest(&b);
    
    Player maxPlayer = PLAYER_BLACK;
    Player minPlayer = PLAYER_WHITE;
    
    char s = 'n';
    uint32_t moveCount = 0;
    while (s != 'c')
    {
        b.state->draw();
        draw(b.state->kingPos);
        b.state->calculateNextMoves(maxPlayer);
        
        if (moveCount >= b.state->childCount)
        {
            moveCount = 0;
        }
        
        GameState* next = b.state->firstChild;
        for (uint32_t i = 0; i < moveCount; i++)
        {
            next = next->nextSibling;
        }
        
        b.state = next;
        Player tempPlayer = maxPlayer;
        maxPlayer = minPlayer;
        minPlayer = tempPlayer;
        moveCount++;
        
        std::cout << b.state->evaluate();
        
        s = getchar();
    }
    
    /*
        b.state->draw();
    b.state->calculateNextMoves(maxPlayer);
    
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
    m->resulting->calculateNextMoves(minPlayer);
    
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
    */
    return 0;
}
