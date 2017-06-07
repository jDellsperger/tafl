#include "Gamestate.h"

void GameState::draw()
{
    for (int16_t y = 0; y < DIM; y++)
    {
        for (int16_t x = 0; x < DIM; x++)
        {
            Vector2 pos = {x, y};
            std::string status;
            FieldState f = this->getStateAtPos(pos);
            
            if (f == FIELD_KING)
            {
                status = "K";
            }
            else if (Board::isFieldPosTarget(pos))
            {
                status = "T";
            }
            else if (f == FIELD_WHITE)
            {
                status = "W";
            }
            else if (f == FIELD_BLACK)
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

void GameState::calculateNextMaxPlayerMoves()
{
    Board* b = Board::getInstance();
    
    // TODO(jan): only do this if next max player moves have not yet been calculated!
    // TODO(jan): extract this somewhere useful
    Vector2 directions[] = {
        {0, 1},
        {0, -1},
        {1, 0},
        {-1, 0}
    };
    
    // Iterate over all fields to find black tokens
    for (uint8_t col = 0; col < DIM; col++)
    {
        for (uint8_t row = 0; row < DIM; row++)
        {
            Vector2 startPos = {row, col};
            
            // If there is a black token, calculate moves
            // for free fields up, down, left and right
            if (this->hasStateAtPos(startPos, FIELD_BLACK))
            {
                for (int moveDirIndex = 0; moveDirIndex < 4; moveDirIndex++)
                { 
                    // Direction we are testing in
                    Vector2 moveDir = directions[moveDirIndex];
                    // Position the token ends up in
                    Vector2 endPos = add(startPos, moveDir);
                    
                    // As long as the end position is on the board and
                    // there is no token on the end position, continue
                    // movement in the given direction
                    while (Board::isFieldPosValid(endPos) && 
                           this->hasStateAtPos(endPos, FIELD_EMPTY))
                    {
                        // If the field is either a target or the throne,
                        // we are not allowed to place the token there
                        if (!Board::isFieldPosTarget(endPos) &&
                            !Board::isFieldPosThrone(endPos))
                        {
                            // Copy the current state to a new gamestate
                            GameState resulting = GameState();
                            this->copyFieldsTo(&resulting);
                            resulting.kingPos = this->kingPos;
                            resulting.zobristHash = this->zobristHash;
                            
                            // Remove token from start position
                            resulting.setStateAtPos(startPos, FIELD_EMPTY);
                            
                            // XOR out the token at the start position from hash
                            uint16_t zStartVal = b->getZobristValue(startPos, FIELD_BLACK);
                            resulting.zobristHash ^= zStartVal;
                            
                            // Add token to end position
                            resulting.setStateAtPos(endPos, FIELD_BLACK);
                            
                            // XOR in the token at the end position to hash
                            uint16_t zEndVal = b->getZobristValue(endPos, FIELD_BLACK);
                            resulting.zobristHash ^= zEndVal;
                            
                            // Test if an enemy token was captured
                            for (int capDirIndex = 0; capDirIndex < 4; capDirIndex++)
                            {
                                // The direction we are testing a capture in
                                Vector2 capDir = directions[capDirIndex];
                                // The position where an ally should be for a capture
                                Vector2 allyPos = add(endPos, scalarMultiply(capDir, 2));
                                
                                // Check if the ally position is valid
                                if (Board::isFieldPosValid(allyPos))
                                {
                                    // The position captured if there is an enemy and the Field
                                    // (this is always valid since the allyPos is farther away)
                                    Vector2 enemyPos = add(endPos, capDir);
                                    
                                    // Test if there actually is an enemy
                                    if (resulting.hasStateAtPos(enemyPos, FIELD_WHITE))
                                    {
                                        // Test if there actually is an ally
                                        if (resulting.hasStateAtPos(allyPos, FIELD_BLACK))
                                        {
                                            // Remove the captured enemy
                                            resulting.setStateAtPos(enemyPos, FIELD_EMPTY);
                                            
                                            // XOR out the enemy from hash
                                            uint16_t zEnemyVal = b->getZobristValue(enemyPos, 
                                                                                    FIELD_WHITE);
                                            resulting.zobristHash ^= zEnemyVal;
                                        }
                                    }
                                }
                            }
                            
                            // See if the resulting gamestate already 
                            // exists in zobrist hash table. If not,
                            // add it.
                            GameState* existing = b->getGameState(resulting.zobristHash,
                                                                  &resulting);
                            if (!existing)
                            {
                                existing = b->addGameState(resulting.zobristHash,
                                                           &resulting);
                            }
                            
                            // Add the resulting gamestate to the possible max player 
                            // moves of the current one
                            Move* m = new Move();
                            m->resulting = existing;
                            m->nextAlternative = this->firstMaxPlayerMove;
                            this->firstMaxPlayerMove = m;
                            
                            this->maxPlayerMoveCount++;
                        }
                        
                        endPos.add(moveDir);
                    }
                }
            }
        }
    }
    
    this->maxPlayerMovesCalculated = true;
}

void GameState::calculateNextMinPlayerMoves()
{
    Board* b = Board::getInstance();
    
    // TODO(jan): only do this if next min player moves have not yet been calculated!
    // TODO(jan): extract this somewhere useful
    Vector2 directions[] = {
        {0, 1},
        {0, -1},
        {1, 0},
        {-1, 0}
    };
    
    // Iterate over all fields to find white tokens
    for (uint8_t col = 0; col < DIM; col++)
    {
        for (uint8_t row = 0; row < DIM; row++)
        {
            Vector2 startPos = {row, col};
            
            // If there is a white token, calculate moves
            // for free fields up, down, left and right
            if (this->hasStateAtPos(startPos, FIELD_WHITE))
            {
                for (int moveDirIndex = 0; moveDirIndex < 4; moveDirIndex++)
                {
                    // Direction we are testing in
                    Vector2 moveDir = directions[moveDirIndex];
                    // Position the token ends up in
                    Vector2 endPos = add(startPos, moveDir);
                    
                    // As long as the end position is on the board and
                    // there is no token on the end position, continue
                    // movement in the given direction
                    while (Board::isFieldPosValid(endPos) && 
                           this->hasStateAtPos(endPos, FIELD_EMPTY))
                    {
                        // If the field is either a target or the throne,
                        // we are not allowed to place the token there
                        if (!Board::isFieldPosTarget(endPos) &&
                            !Board::isFieldPosThrone(endPos))
                        {
                            // Copy the current state to a new gamestate
                            GameState resulting = GameState();
                            this->copyFieldsTo(&resulting);
                            resulting.kingPos = this->kingPos;
                            resulting.zobristHash = this->zobristHash;
                            
                            // Remove token from start position
                            resulting.setStateAtPos(startPos, FIELD_EMPTY);
                            
                            // XOR out the token at the start position from hash
                            uint16_t zStartVal = b->getZobristValue(startPos, FIELD_WHITE);
                            resulting.zobristHash ^= zStartVal;
                            
                            // Add token to end position
                            resulting.setStateAtPos(endPos, FIELD_WHITE);
                            
                            // XOR in the token at the end position to hash
                            uint16_t zEndVal = b->getZobristValue(endPos, FIELD_WHITE);
                            resulting.zobristHash ^= zEndVal;
                            
                            // Test if an enemy token was captured
                            for (int capDirIndex = 0; capDirIndex < 4; capDirIndex++)
                            {
                                // The direction we are testing a capture in
                                Vector2 capDir = directions[capDirIndex];
                                // The position where an ally should be for a capture
                                Vector2 allyPos = add(endPos, scalarMultiply(capDir, 2));
                                
                                // Check if the ally position is valid
                                if (Board::isFieldPosValid(allyPos))
                                {
                                    // The position captured if there is an enemy and the Field
                                    // (this is always valid since the allyPos is farther away)
                                    Vector2 enemyPos = add(endPos, capDir);
                                    
                                    // Test if there actually is an enemy
                                    if (resulting.hasStateAtPos(enemyPos, FIELD_BLACK))
                                    {
                                        // Test if there actually is an ally
                                        if (resulting.hasStateAtPos(allyPos, FIELD_WHITE) ||
                                            resulting.hasStateAtPos(allyPos, FIELD_KING))
                                        {
                                            // Remove the captured enemy
                                            resulting.setStateAtPos(enemyPos, FIELD_EMPTY);
                                            
                                            // XOR out the enemy from hash
                                            uint16_t zEnemyVal = b->getZobristValue(enemyPos, 
                                                                                    FIELD_BLACK);
                                            resulting.zobristHash ^= zEnemyVal;
                                        }
                                    }
                                }
                            }
                            
                            // See if the resulting gamestate already 
                            // exists in zobrist hash table. If not,
                            // add it.
                            GameState* existing = b->getGameState(resulting.zobristHash,
                                                                  &resulting);
                            if (!existing)
                            {
                                existing = b->addGameState(resulting.zobristHash,
                                                           &resulting);
                            }
                            
                            // Add the resulting gamestate to the possible max player 
                            // moves of the current one
                            Move* m = new Move();
                            m->resulting = existing;
                            m->nextAlternative = this->firstMinPlayerMove;
                            this->firstMinPlayerMove = m;
                            
                            this->minPlayerMoveCount++;
                        }
                        
                        endPos.add(moveDir);
                    }
                }
            }
            else if (this->hasStateAtPos(startPos, FIELD_KING))
            {
                for (int moveDirIndex = 0; moveDirIndex < 4; moveDirIndex++)
                {
                    // Direction we are testing in
                    Vector2 moveDir = directions[moveDirIndex];
                    // Position the token ends up in
                    Vector2 endPos = add(startPos, moveDir);
                    
                    // As long as the end position is on the board and
                    // there is no token on the end position, continue
                    // movement in the given direction
                    while (Board::isFieldPosValid(endPos) && 
                           this->hasStateAtPos(endPos, FIELD_EMPTY))
                    {
                        // If the field is the throne,
                        // we are not allowed to place the token there
                        if (!Board::isFieldPosThrone(endPos))
                        {
                            // Copy the current state to a new gamestate
                            GameState resulting = GameState();
                            this->copyFieldsTo(&resulting);
                            resulting.kingPos = endPos;
                            resulting.zobristHash = this->zobristHash;
                            
                            // Remove token from start position
                            resulting.setStateAtPos(startPos, FIELD_EMPTY);
                            
                            // XOR out the token at the start position from hash
                            uint16_t zStartVal = b->getZobristValue(startPos, FIELD_KING);
                            resulting.zobristHash ^= zStartVal;
                            
                            // Add token to end position
                            resulting.setStateAtPos(endPos, FIELD_KING);
                            
                            // XOR in the token at the end position to hash
                            uint16_t zEndVal = b->getZobristValue(endPos, FIELD_KING);
                            resulting.zobristHash ^= zEndVal;
                            
                            // Test if an enemy token was captured
                            for (int capDirIndex = 0; capDirIndex < 4; capDirIndex++)
                            {
                                // The direction we are testing a capture in
                                Vector2 capDir = directions[capDirIndex];
                                // The position where an ally should be for a capture
                                Vector2 allyPos = add(endPos, scalarMultiply(capDir, 2));
                                
                                // Check if the ally position is valid
                                if (Board::isFieldPosValid(allyPos))
                                {
                                    // The position captured if there is an enemy
                                    // (this is always valid since the allyPos is farther away)
                                    Vector2 enemyPos = add(endPos, capDir);
                                    
                                    // Test if there actually is an enemy
                                    if (resulting.hasStateAtPos(enemyPos, FIELD_BLACK))
                                    {
                                        // Test if there actually is an ally
                                        if (resulting.hasStateAtPos(allyPos, FIELD_WHITE))
                                        {
                                            // Remove the captured enemy
                                            resulting.setStateAtPos(enemyPos, FIELD_EMPTY);
                                            
                                            // XOR out the enemy from hash
                                            uint16_t zEnemyVal = b->getZobristValue(enemyPos, 
                                                                                    FIELD_BLACK);
                                            resulting.zobristHash ^= zEnemyVal;
                                        }
                                    }
                                }
                            }
                            
                            // See if the resulting gamestate already 
                            // exists in zobrist hash table. If not,
                            // add it.
                            GameState* existing = b->getGameState(resulting.zobristHash,
                                                                  &resulting);
                            if (!existing)
                            {
                                existing = b->addGameState(resulting.zobristHash,
                                                           &resulting);
                            }
                            
                            // Add the resulting gamestate to the possible max player 
                            // moves of the current one
                            Move* m = new Move();
                            m->resulting = existing;
                            m->nextAlternative = this->firstMinPlayerMove;
                            this->firstMinPlayerMove = m;
                            
                            this->minPlayerMoveCount++;
                        }
                        
                        endPos.add(moveDir);
                    }
                }
            }
        }
    }
    
    this->minPlayerMovesCalculated = true;
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
    for (int16_t y = min(c.y, t.y); y <= max(c.y, t.y); y++)
    {
        for (int16_t x = min(c.x, t.x); x <= max(c.x, t.x); x++)
        {
            Vector2 pos = {x, y};
            
            if (this->hasStateAtPos(pos, FIELD_WHITE))
            {
                //*val;
            }
            else if (this->hasStateAtPos(pos, FIELD_BLACK))
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
        if (Board::isFieldPosValid(capPos))
        {
            if (!this->hasStateAtPos(capPos, FIELD_BLACK))
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
        
        if (player == PLAYER_MAX)
        {
            if (!this->maxPlayerMovesCalculated)
            {
                this->calculateNextMaxPlayerMoves();
            }
            
            if (this->maxPlayerMoveCount == 0)
            {
                this->val = this->evaluate();
            }
            else
            {
                Move* m = this->firstMaxPlayerMove;
                this->val = INT16_MIN;
                
                while (m != nullptr)
                {
                    GameState* s = m->resulting;
                    s->minimax(cutOff, PLAYER_MIN);
                    
                    if (this->val < s->val)
                    {
                        this->val = s->val;
                    }
                    
                    m = m->nextAlternative;
                }
            }
        }
        else
        {
            if (!this->minPlayerMovesCalculated)
            {
                this->calculateNextMinPlayerMoves();
            }
            
            if (this->minPlayerMoveCount == 0)
            {
                this->val = this->evaluate();
            }
            else
            {
                Move* m = this->firstMinPlayerMove;
                this->val = INT16_MAX;
                
                while (m != nullptr)
                {
                    GameState* s = m->resulting;
                    s->minimax(cutOff, PLAYER_MAX);
                    
                    if (this->val > s->val)
                    {
                        this->val = s->val;
                    }
                    
                    m = m->nextAlternative;
                }
            }
        }
    }
}

void GameState::generateZobristHash()
{
    Board* b = Board::getInstance();
    uint16_t hash = 0;
    
    for (int16_t y = 0; y < DIM; y++)
    {
        for (int16_t x = 0; x < DIM; x++)
        {
            Vector2 fieldPos = {x, y};
            FieldState s = this->getStateAtPos(fieldPos);
            
            int zobristValue = b->getZobristValue(fieldPos, s);
            hash = hash ^ zobristValue;
        }
    }
    
    this->zobristHash = hash;
}
