#include "Gamestate.h"

Vector2 Board::thronePos = {(int)(DIM/2), (int)(DIM/2)};

void GameState::draw()
{
    for (int16_t y = 0; y < DIM; y++)
    {
        for (int16_t x = 0; x < DIM; x++)
        {
            Vector2 pos = {x, y};
            std::string status;
            Field* f = this->getFieldAtPos(pos);
            
            if (f->hasState(FIELD_KING))
            {
                status = "K";
            }
            else if (Board::isFieldPosTarget(pos))
            {
                status = "T";
            }
            else if (f->hasState(FIELD_WHITE))
            {
                status = "W";
            }
            else if (f->hasState(FIELD_BLACK))
            {
                status = "B";
            }
            else if (Board::isFieldPosThrone(pos))
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
        allyFlags = FIELD_BLACK;
        enemyFlags = FIELD_WHITE;
    }
    else
    {
        allyFlags = (FIELD_WHITE | FIELD_KING);
        enemyFlags = FIELD_BLACK;
    }
    
    Vector2 allyFieldP = add(testFieldP, scalarMultiply(testDir, 2));
    if (this->isFieldPosValid(allyFieldP))
    {
        Vector2 enemyFieldP = add(testFieldP, testDir);
        Field* enemyField = this->getFieldAtPos(enemyFieldP);
        if (enemyField->hasOneOfFlags(enemyFlags))
        {
            Field* allyField = this->getFieldAtPos(allyFieldP);
            if (allyField->hasOneOfFlags(allyFlags))
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
        allyFlags = FIELD_BLACK;
    }
    else
    {
        allyFlags = (FIELD_WHITE | FIELD_KING);
    }
    
    Vector2 start = {row, col};
    // NOTE(jan): get the actual flag set on the field
    Field* originalStartField = this->getFieldAtPos(start);
    uint8_t allyFlagAtStart = originalStartField->flags & allyFlags;
    Vector2 end = add(start, dir);
    
    Field* testField = this->getFieldAtPos(end);
    while (this->isFieldPosValid(end) && testField->hasState(FIELD_EMPTY))
    {
        // NOTE(jan): If the field is either a target or the throne,
        // we are not allowed to place the token there
        if (originalStartField->hasState(FIELD_KING) ||
            (!Board::isFieldPosTarget(end) &&
             !Board::isFieldPosThrone(end)))
        {
            // TODO(jan): add new gamestate to zobrist hash-table if necessary
            GameState* resulting = new GameState();
            this->copyFieldsTo(resulting);
            
            Field* startField = resulting->getFieldAtPos(start);
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
            
            if (allyFlagAtStart == FIELD_KING)
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
        allyFlags = FIELD_BLACK;
    }
    else
    {
        allyFlags = (FIELD_WHITE | FIELD_KING);
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
            if (f->hasOneOfFlags(allyFlags))
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
            int temp = this->calcQuadrantValue(this->kingPos, targets[i]);
            
            if (val > temp)
            {
                target = targets[i];
                val= temp;
            }
            
        }
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
            
            if (f.hasOneOfFlags(FIELD_WHITE))
            {
                //*val;
            }
            else if (f.hasOneOfFlags(FIELD_BLACK))
            {
                val = val + 2;
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
            if (!capField->hasState(FIELD_BLACK))
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
    if (Board::isFieldPosTarget(this->kingPos))
    {
        victor = PLAYER_MIN;
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
        this->calculateNextMoves(player);
        
        // If there are no possible moves, return own value
        if (this->childCount == 0)
        {
            this->val = this->evaluate();
        }
        else
        {
            GameState* s = this->firstChild;
            
            if (player == PLAYER_MAX)
            {
                this->val = INT16_MIN;
                
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
}
