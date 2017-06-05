#include <string>
#include <iostream>
#include <climits>

#include "Math.h"
#include "Field.cpp"
#include "Gamestate.cpp"

void initBrandubh(Board* b)
{
    b->state = new GameState();
    
    b->state->fields[0][3].setFlags(FIELD_BLACK);
    b->state->fields[1][3].setFlags(FIELD_BLACK);
    b->state->fields[5][3].setFlags(FIELD_BLACK);
    b->state->fields[6][3].setFlags(FIELD_BLACK);
    b->state->fields[3][0].setFlags(FIELD_BLACK);
    b->state->fields[3][1].setFlags(FIELD_BLACK);
    b->state->fields[3][5].setFlags(FIELD_BLACK);
    b->state->fields[3][6].setFlags(FIELD_BLACK);
    
    b->state->fields[3][2].setFlags(FIELD_WHITE);
    b->state->fields[3][4].setFlags(FIELD_WHITE);
    b->state->fields[2][3].setFlags(FIELD_WHITE);
    b->state->fields[4][3].setFlags(FIELD_WHITE);
    b->state->fields[3][3].setFlags(FIELD_KING);
    
    b->state->kingPos = {3, 3};
}

void initTest(Board* b)
{
    b->state = new GameState();
    b->state->fields[3][4].setFlags(FIELD_BLACK);
    b->state->fields[4][3].setFlags(FIELD_BLACK);
    b->state->fields[2][3].setFlags(FIELD_BLACK);
    b->state->fields[2][2].setFlags(FIELD_BLACK);
    
    b->state->fields[3][3].setFlags(FIELD_KING);
    
    b->state->kingPos = {3, 3};
}

int main()
{
    Board b;
    initBrandubh(&b);
    //initTest(&b);
    
    Player activePlayer = PLAYER_MAX;
    Player inactivePlayer = PLAYER_MIN;
    
    char s = 'n';
    uint32_t moveCount = 0;
    while (s != 'c')
    {
        std::cout << "------------------" << std::endl;
        
        if (activePlayer == PLAYER_MAX)
        {
            std::cout << "max player active" << std::endl;
        }
        else
        {
            std::cout << "min player active" << std::endl;
        }
        
        b.state->draw();
        
        Player victor = b.state->checkVictory();
        if (victor == PLAYER_MAX)
        {
            std::cout << "Max player won!!!" << std::endl;
            getchar();
            break;
        }
        else if (victor == PLAYER_MIN)
        {
            std::cout << "Min player won!!!" << std::endl;
            getchar();
            break;
        }
        
        b.state->minimax(4, activePlayer);
        std::cout << "Minimax value: " << b.state->val << std::endl;
        std::cout << "King position: ";
        draw(b.state->kingPos);
        std::cout << std::endl;
        
        if (moveCount >= b.state->childCount)
        {
            moveCount = 0;
        }
        
        GameState* next = b.state->firstChild;
        GameState* candidate = next;
        if (next != nullptr) {
            for (uint32_t i = 0; i < b.state->childCount; i++)
            {
                if (activePlayer == PLAYER_MAX)
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
        }
        else
        {
            std::cout << "No more moves..." << std::endl;
            getchar();
            break;
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
