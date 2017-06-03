#include "Gamestate.h"

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

/* 
NOTE(jan): Tests if a capture-situation (ally-enemy-ally) is present in the
given direction
*/
void GameState::testCaptureInDirection(Player player,
                                       Vector2 testFieldP, 
                                       Vector2 testDir)
{
    uint8_t allyFlags, enemyFlags;
    if (player == PLAYER_MAX)
    {
        allyFlags = BLACK;
        enemyFlags = WHITE;
    }
    else
    {
        allyFlags = (WHITE | KING);
        enemyFlags = BLACK;
    }
    
    Vector2 allyFieldP = add(testFieldP, scalarMultiply(testDir, 2));
    if (this->isFieldPosValid(allyFieldP))
    {
        Vector2 enemyFieldP = add(testFieldP, testDir);
        Field* enemyField = this->getFieldAtPos(enemyFieldP);
        if (enemyField->hasFlags(enemyFlags))
        {
            Field* allyField = this->getFieldAtPos(allyFieldP);
            if (allyField->hasFlags(allyFlags))
            {
                enemyField->removeFlags(enemyFlags);
            }
        }
    }
}

/* 
NOTE(jan): Calculates possible moves from field at position col-row
in a certain direction
*/
void GameState::calculateMovesInDirection(Player player,
                                          uint8_t row, 
                                          uint8_t col, 
                                          Vector2 dir)
{
    uint8_t allyFlags;
    if (player == PLAYER_MAX)
    {
        allyFlags = BLACK;
    }
    else
    {
        allyFlags = (WHITE | KING);
    }
    
    Vector2 start = {row, col};
    Vector2 end = add(start, dir);
    
    Field* testField = this->getFieldAtPos(end);
    while (this->isFieldPosValid(end) && !testField->hasFlags(WHITE | BLACK | KING))
    {
        if (!testField->hasFlags(BLOCKING))
        {
            GameState* resulting = new GameState();
            this->copyFieldsTo(resulting);
            
            Field* startField = resulting->getFieldAtPos(start);
            // NOTE(jan): get the actual flag set on the field
            uint8_t allyFlagAtStart = startField->flags & allyFlags;
            startField->removeFlags(allyFlags);
            resulting->getFieldAtPos(end)->setFlags(allyFlagAtStart);
            
            // NOTE(jan): Check if an enemy token was captured
            
            // Test up
            Vector2 testDir = {0, -1};
            resulting->testCaptureInDirection(player, end, testDir);
            
            // Test right
            testDir = {1, 0};
            resulting->testCaptureInDirection(player, end, testDir);
            
            // Test down
            testDir = {0, 1};
            resulting->testCaptureInDirection(player, end, testDir);
            
            // Test left
            testDir = {-1, 0};
            resulting->testCaptureInDirection(player, end, testDir);
            
            if (allyFlagAtStart == KING)
            {
                resulting->kingPos = end;
            }
            else
            {
                resulting->kingPos = this->kingPos;
            }
            // TODO(jan): Move this out to its proper method?
            resulting->parent = this;
            resulting->nextSibling = this->firstChild;
            this->firstChild = resulting;
            
            this->childCount++;
        }
        
        end.add(dir);
        testField = this->getFieldAtPos(end);
    }
}

void GameState::calculateNextMoves(Player player)
{
    uint8_t allyFlags;
    if (player == PLAYER_MAX)
    {
        allyFlags = BLACK;
    }
    else
    {
        allyFlags = (WHITE | KING);
    }
    
    //NOTE(jan): Iterate over all fields to find black tokens
    for (uint8_t col = 0; col < DIM; col++)
    {
        for (uint8_t row = 0; row < DIM; row++)
        {
            Vector2 p = {row, col};
            Field* f = this->getFieldAtPos(p);
            
            // NOTE(jan): If there is an allied token, calculate moves
            // for free fields up, down, left and right
            if (f->hasFlags(allyFlags))
            {
                // Test up
                Vector2 dir = {0, -1};
                this->calculateMovesInDirection(player, row, col, dir);
                
                // Test right
                dir = {1, 0};
                this->calculateMovesInDirection(player, row, col, dir);
                
                // Test down;
                dir = {0, 1};
                this->calculateMovesInDirection(player, row, col, dir);
                
                // Test left
                dir = {-1, 0};
                this->calculateMovesInDirection(player, row, col, dir);
            }
        }
    }
}

int16_t GameState::evaluate()
{
    int val = INT_MAX;
    
    Player victor = this->checkVictory();
    if (victor == PLAYER_MIN)
    {
        val = -10;
    }
    else if (victor == PLAYER_MAX)
    {
        val = 10;
    }
    else {
        Vector2 target;
        
        Vector2 tl = { 0, 0 };
        Vector2 tr = { DIM-1, 0 };
        Vector2 dl = { 0, DIM-1 };
        Vector2 dr = { DIM-1, DIM-1 };
        
        Vector2 targets[] = { tl, tr, dl, dr };
        
        for (int i = 0; i < 4; i++)
        {
            //std::cout << "Quadrant " << i << ":" << std::endl;
            int temp = this->calcQuadrantValue(this->kingPos, targets[i]);
            
            if (val > temp)
            {
                target = targets[i];
                val= temp;
            }
            
        }
        
        //std::cout << "Target is: " << target.x << " | " << target.y << std::endl;
    }
    
    return val;
}

int16_t GameState::calcQuadrantValue(Vector2 c, Vector2 t)
{
    int16_t val = 0;
    for (int y = min(c.y, t.y); y <= max(c.y, t.y); y++)
    {
        for (int x = min(c.x, t.x); x <= max(c.x, t.x); x++)
        {
            Field f = this->fields[y][x];
            
            if (f.hasFlags(WHITE))
            {
                //*val;
            }
            else if (f.hasFlags(BLACK))
            {
                val = val + 2;
                
                //std::cout << "Black at: " << x << " | " << y << std::endl;
            }
        }
    }
    
    return val;
}

Player GameState::checkVictory()
{
    Player victor = PLAYER_MAX;
    
    // Check if the max player captured the king
    Vector2 kingCap[4] = {
        add(this->kingPos, {-1, 0}),
        add(this->kingPos, {1, 0}),
        add(this->kingPos, {0, -1}),
        add(this->kingPos, {0, 1})
    };
    
    for (int i = 0; i < 4; i++)
    {
        Vector2 capPos = kingCap[i];
        if (this->isFieldPosValid(capPos))
        {
            Field* capField = this->getFieldAtPos(capPos);
            if (!capField->hasFlags(BLACK))
            {
                victor = PLAYER_NONE;
                break;
            }
        }
        else
        {
            victor = PLAYER_NONE;
            break;
        }
    }
    
    // Check if min player escaped with the king
    Vector2 tPos[4] = {
        {0, 0},
        {DIM - 1, 0},
        {0, DIM - 1},
        {DIM - 1, DIM - 1}
    };
    
    for (int t = 0; t < 4; t++)
    {
        Field* target = this->getFieldAtPos(tPos[t]);
        
        if (target->hasFlags(KING))
        {
            victor = PLAYER_MIN;
            break;
        }
    }
    
    return victor;
}

void GameState::minimax(int cutOff, Player player)
{
    // If cut-off depth is reached, return own value
    if (cutOff == 0) {
        this->val = this->evaluate();
    }
    else
    {
        cutOff--;
        
        if (player == PLAYER_MAX)
        {
            this->val = INT16_MIN;
            
            if (!this->childCount)
            {
                this->calculateNextMoves(PLAYER_MIN);
            }
            
            GameState* s = this->firstChild;
            while (s != nullptr)
            {
                s->minimax(cutOff, PLAYER_MIN);
                
                if (this->val < s->val)
                {
                    this->val = s->val;
                }
                
                s = s->nextSibling;
            }
        }
        else
        {
            this->val = INT16_MAX;
            
            if (!this->childCount)
            {
                this->calculateNextMoves(PLAYER_MAX);
            }
            
            GameState* s = this->firstChild;
            while (s != nullptr)
            {
                s->minimax(cutOff, PLAYER_MAX);
                
                if (this->val > s->val)
                {
                    this->val = s->val;
                }
                
                s = s->nextSibling;
            }
        }
    }
}
