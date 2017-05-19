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
    initBrandubh(&b);
    //initTest(&b);
    
    Player maxPlayer = PLAYER_BLACK;
    Player minPlayer = PLAYER_WHITE;
    Player activePlayer = maxPlayer;
    Player inactivePlayer = minPlayer;
    
    char s = 'n';
    uint32_t moveCount = 0;
    while (s != 'c')
    {
        std::cout << "------------------" << std::endl;
        
        if (activePlayer == maxPlayer)
        {
            std::cout << "max player active" << std::endl;
        }
        else
        {
            std::cout << "min player active" << std::endl;
        }
        
        b.state->draw();
        draw(b.state->kingPos);
        std::cout << b.state->evaluate();
        b.state->calculateNextMoves(activePlayer);
        
        if (moveCount >= b.state->childCount)
        {
            moveCount = 0;
        }
        
        GameState* next = b.state->firstChild;
        GameState* candidate = next;
        for (uint32_t i = 0; i < b.state->childCount; i++)
        {
            if (activePlayer == maxPlayer)
            {
                if (next->val > candidate->val)
                {
                    candidate = next;
                }
            }
            else
            {
                if (next->val < candidate->val)
                {
                    candidate = next;
                }
            }
            
            next = next->nextSibling;
        }
        
        b.state = candidate;
        Player tempPlayer = activePlayer;
        activePlayer = inactivePlayer;
        inactivePlayer = tempPlayer;
        moveCount++;
        
        s = getchar();
    }
    
    
    
    return 0;
}
