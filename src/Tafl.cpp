#include <string>
#include <iostream>
#include <climits>
#include <cstdlib>

#include "Math.h"
#include "Board.cpp"
#include "Gamestate.cpp"

void initBrandubh(Board* b)
{
    b->state = new GameState();
    
    b->state->fields[0][3] = FIELD_BLACK;
    b->state->fields[1][3] = FIELD_BLACK;
    b->state->fields[5][3] = FIELD_BLACK;
    b->state->fields[6][3] = FIELD_BLACK;
    b->state->fields[3][0] = FIELD_BLACK;
    b->state->fields[3][1] = FIELD_BLACK;
    b->state->fields[3][5] = FIELD_BLACK;
    b->state->fields[3][6] = FIELD_BLACK;
    
    b->state->fields[3][2] = FIELD_WHITE;
    b->state->fields[3][4] = FIELD_WHITE;
    b->state->fields[2][3] = FIELD_WHITE;
    b->state->fields[4][3] = FIELD_WHITE;
    b->state->fields[3][3] = FIELD_KING;
    
    b->state->kingPos = {3, 3};
    b->state->generateZobristHash();
}

void initTest(Board* b)
{
    b->state = new GameState();
    b->state->fields[3][4] = FIELD_BLACK;
    b->state->fields[4][3] = FIELD_BLACK;
    b->state->fields[2][3] = FIELD_BLACK;
    b->state->fields[2][2] = FIELD_BLACK;
    
    b->state->fields[3][3] = FIELD_KING;
    
    b->state->kingPos = {3, 3};
    b->state->generateZobristHash();
}

void initTest2(Board* b)
{
    b->state = new GameState();
    b->state->fields[2][2] = FIELD_BLACK;
    
    b->state->fields[3][3] = FIELD_KING;
    
    b->state->kingPos = {3, 3};
    b->state->generateZobristHash();
}

int main()
{
    Board* b = Board::getInstance();
    initBrandubh(b);
    //initTest2(b);
    
    b->state->draw();
    
    Player activePlayer = PLAYER_MAX;
    Player inactivePlayer = PLAYER_MIN;
    
    char s = 'n';
    while (s != 'c')
    {
        s = getchar();
        
        b->roundCount++;
        
        //b->state->minimax(4, activePlayer);
        b->state->abPruning(5, activePlayer, -100, 100);
        GameState* candidate = nullptr;
        
        if (activePlayer == PLAYER_MAX)
        {
            Move* m = b->state->firstMaxPlayerMove;
            
            if (m != nullptr) {
                candidate = m->resulting;
                
                while (m)
                {
                    GameState* s = m->resulting;
                    if (s->val > candidate->val)
                    {
                        candidate = s;
                    }
                    
                    m = m->nextAlternative;
                }
            }
            else
            {
                std::cout << "No more moves..." << std::endl;
                getchar();
                break;
            }
        }
        else
        {
            Move* m = b->state->firstMinPlayerMove;
            
            if (m != nullptr) {
                candidate = m->resulting;
                
                while (m)
                {
                    GameState* s = m->resulting;
                    if (s->val < candidate->val)
                    {
                        candidate = s;
                    }
                    
                    m = m->nextAlternative;
                }
            }
            else
            {
                std::cout << "No more moves..." << std::endl;
                getchar();
                break;
            }
        }
        
        b->state = candidate;
        Player tempPlayer = activePlayer;
        activePlayer = inactivePlayer;
        inactivePlayer = tempPlayer;
        
        std::cout << "------------------" << std::endl;
        std::cout << "Round " + std::to_string(b->roundCount) + ":" 
            << std::endl << std::endl;
        
        b->state->draw();
        
        Player victor = b->state->checkVictory();
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
        
        std::cout << "Minimax value: " << b->state->val << std::endl;
        std::cout << "King position: ";
        draw(b->state->kingPos);
        
        std::cout << "Zobrist hash: " << b->state->zobristHash << std::endl;
        std::cout << b->state->info << std::endl;
        std::cout << "Hopcount: " << std::to_string(b->state->calcHops()) << std::endl;
        
        if (activePlayer == PLAYER_MAX)
        {
            std::cout << "Max Player moves next" << std::endl;
        }
        else
        {
            std::cout << "Min Player moves next" << std::endl;
        }
    }
    
    return 0;
}
