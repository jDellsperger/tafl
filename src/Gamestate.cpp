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

void GameState::setStateAtPos(Vector2 pos, FieldState s, Board* b)
{
    // Update zobrist hash by XOR-ing old value out
    FieldState oldState = this->fields[pos.y][pos.x];
    uint16_t oldZobristVal = b->getZobristValue(pos, oldState);
    this->zobristHash ^= oldZobristVal;
    
    // Set new state
    this->fields[pos.y][pos.x] = s;
    
    // Update zobrist hash by XOR-ing new value in
    uint16_t newZobristVal = b->getZobristValue(pos, s);
    this->zobristHash ^= newZobristVal;
}

void GameState::addMinPlayerMove(GameState* resulting)
{
    Move* m = new Move();
    m->resulting = resulting;
    m->nextAlternative = this->firstMinPlayerMove;
    this->firstMinPlayerMove = m;
    
    this->minPlayerMoveCount++;
}

void GameState::addMaxPlayerMove(GameState* resulting)
{
    Move* m = new Move();
    m->resulting = resulting;
    m->nextAlternative = this->firstMaxPlayerMove;
    this->firstMaxPlayerMove = m;
    
    this->maxPlayerMoveCount++;
}

// Tests if there is a capture situation present for a given position
// allyState can be multiple FieldStates OR-ed together.
void GameState::testCaptureAtPos(Vector2 pos,
                                 uint8_t allyState, 
                                 FieldState enemyState, 
                                 Board* b)
{
    for (int capDirIndex = 0; capDirIndex < 4; capDirIndex++)
    {
        Vector2 capDir = DIRECTIONS[capDirIndex];
        Vector2 allyPos = add(pos, scalarMultiply(capDir, 2));
        
        if (Board::isFieldPosValid(allyPos))
        {
            Vector2 enemyPos = add(pos, capDir);
            
            if (this->hasStateAtPos(enemyPos, enemyState))
            {
                if (this->getStateAtPos(allyPos) & allyState)
                {
                    this->setStateAtPos(enemyPos, FIELD_EMPTY, b);
                }
            }
        }
    }
}

void GameState::calculateNextMaxPlayerMoves()
{
    Board* b = Board::getInstance();
    
    // TODO(jan): only do this if next max player moves have not yet been calculated!
    
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
                    Vector2 moveDir = DIRECTIONS[moveDirIndex];
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
                            
                            // Move token from start to end pos
                            resulting.setStateAtPos(startPos, FIELD_EMPTY, b);
                            resulting.setStateAtPos(endPos, FIELD_BLACK, b);
                            
                            resulting.testCaptureAtPos(endPos, FIELD_BLACK, FIELD_WHITE, b);
                            
                            // Get resulting gamestate from zobrist hash table
                            GameState* existing = b->getZobristAddress(&resulting);
                            
                            // Add the resulting gamestate to possible moves
                            this->addMaxPlayerMove(existing);
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
                    Vector2 moveDir = DIRECTIONS[moveDirIndex];
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
                            
                            // Move token from start to end
                            resulting.setStateAtPos(startPos, FIELD_EMPTY, b);
                            resulting.setStateAtPos(endPos, FIELD_WHITE, b);
                            
                            // Test if an enemy was captured
                            resulting.testCaptureAtPos(endPos, 
                                                       (FIELD_WHITE | FIELD_KING),
                                                       FIELD_BLACK, b);
                            
                            // Get resulting gamestate from zobrist hash table
                            GameState* existing = b->getZobristAddress(&resulting);
                            
                            // Add the resulting gamestate to possible moves
                            this->addMinPlayerMove(existing);
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
                    Vector2 moveDir = DIRECTIONS[moveDirIndex];
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
                            resulting.setStateAtPos(startPos, FIELD_EMPTY, b);
                            
                            // Add token to end position
                            resulting.setStateAtPos(endPos, FIELD_KING, b);
                            
                            // Test if an enemy token was captured
                            resulting.testCaptureAtPos(endPos, FIELD_WHITE, FIELD_BLACK, b);
                            
                            // Get resulting gamestate from zobrist hash table
                            GameState* existing = b->getZobristAddress(&resulting);
                            
                            // Add the resulting gamestate to possible moves
                            this->addMinPlayerMove(existing);
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
    else if (victor == PLAYER_BOTH)
    {
        val = 0;
    }
    else {
        // Hops to target, [-6,6]
        int hopCount = this->calcHops();
        if (hopCount > 6)
        {
            hopCount = 6;
        }
        
        hopCount *= 2;
        hopCount -= 6;
        
        // Diff white and black tokens, [-2,2]
        int tokenDiff = this->calcTokenDiff();
        if (tokenDiff > 2)
        {
            tokenDiff = 2;
        }
        else if (tokenDiff < -2)
        {
            tokenDiff = -2;
        }
        
        // King degrees of freedom, [-2, 2]
        int kingFreedom = this->calcKingFreedom();
        kingFreedom *= 4/3;
        kingFreedom -= 2;
        
        val = hopCount + tokenDiff + kingFreedom;
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
    Player victor = PLAYER_NONE;
    
    // Check if there is a tie
    if (this->repetitionCount >= 3)
    {
        victor = PLAYER_BOTH;
    }
    
    // Check if the max player captured the king
    Vector2 kingCap[4] = {
        add(this->kingPos, {-1, 0}),
        add(this->kingPos, {1, 0}),
        add(this->kingPos, {0, -1}),
        add(this->kingPos, {0, 1})
    };
    
    bool maxCondition = true;
    for (int i = 0; i < 4; i++)
    {
        Vector2 capPos = kingCap[i];
        if (Board::isFieldPosValid(capPos))
        {
            if (!this->hasStateAtPos(capPos, FIELD_BLACK))
            {
                maxCondition = false;
                break;
            }
        }
        else
        {
            maxCondition = false;
            break;
        }
    }
    
    if (maxCondition)
    {
        victor = PLAYER_MAX;
    }
    
    // Check if min player escaped with the king
    if (Board::isFieldPosTarget(this->kingPos))
    {
        victor = PLAYER_MIN;
    }
    
    return victor;
}

void GameState::abPruning(int cutOff, Player player, int alpha, int beta)
{
    this->repetitionCount++;
    if (this->repetitionCount >= 3)
    {
        this->val = 0;
        this->repetitionCount--;
        return;
    }
    
    if (cutOff > 0)
    {
        cutOff--;
        
        if (player == PLAYER_MAX)
        {
            if (!this->maxPlayerMovesCalculated)
            {
                this->calculateNextMaxPlayerMoves();
            }
            
            if (this->maxPlayerMoveCount > 0)
            {
                Move* m = this->firstMaxPlayerMove;
                while (m != nullptr)
                {
                    GameState* s = m->resulting;
                    s->abPruning(cutOff, PLAYER_MIN, alpha, beta);
                    
                    if (alpha < s->val)
                    {
                        alpha = s->val;
                    }
                    
                    if (alpha >= beta)
                    {
                        this->val = beta;
                        this->repetitionCount--;
                        return;
                    }
                    
                    m = m->nextAlternative;
                }
                
                this->val = alpha;
                this->repetitionCount--;
                return;
            }
        }
        else
        {
            if (!this->minPlayerMovesCalculated)
            {
                this->calculateNextMinPlayerMoves();
            }
            
            if (this->minPlayerMoveCount > 0)
            {
                Move* m = this->firstMinPlayerMove;
                while (m != nullptr)
                {
                    GameState* s = m->resulting;
                    s->abPruning(cutOff, PLAYER_MAX, alpha, beta);
                    
                    if (beta > s->val)
                    {
                        beta = s->val;
                    }
                    
                    if (alpha >= beta)
                    {
                        this->val = alpha;
                        this->repetitionCount--;
                        return;
                    }
                    
                    m = m->nextAlternative;
                }
                
                this->val = beta;
                this->repetitionCount--;
                return;
            }
        }
    }
    
    this->val = this->evaluate();
    this->repetitionCount--;
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

int GameState::calcHops()
{
    GraphVertex graph[DIM][DIM];
    
    for (int16_t y = 0; y < DIM; y++)
    {
        for (int16_t x = 0; x < DIM; x++)
        {
            GraphVertex* v = &graph[y][x];
            v->index = y * DIM + x;
            
            for (int i = 0; i < 4; i++)
            {
                Vector2 p = { x,y };
                Vector2 dir = DIRECTIONS[i];
                p.add(dir);
                
                while (Board::isFieldPosValid(p))
                {
                    FieldState s=  this->getStateAtPos(p);
                    
                    if (s != FIELD_EMPTY)
                    {
                        break;
                    }
                    
                    else if (!Board::isFieldPosThrone(p))
                    {
                        v->edges[v->edgeCount] = &graph[p.y][p.x];
                        v->edgeCount++;
                    }
                    
                    p.add(dir);
                }
            }
        }
    }
    
    GraphVertex* queue[DIM * DIM];
    queue[0] = &graph[this->kingPos.y][this->kingPos.x];
    int queueCount = 1;
    int queueIndex = 0;
    int visitedFrom[DIM * DIM];
    for (int i = 0; i < DIM * DIM; i++)
    {
        visitedFrom[i] = -1;
    }
    
    int reachedTargetIndex = -1;
    
    int targetIndices[4] = {
        0, DIM - 1, DIM * (DIM - 1), (DIM * DIM) - 1
    };
    
    while (queueCount)
    {
        GraphVertex* v = queue[queueIndex];
        
        for (int i = 0; i < v->edgeCount; i++) {
            GraphVertex* u = v->edges[i];
            
            if (visitedFrom[u->index] < 0)
            {
                queue[queueIndex + queueCount] = u;
                visitedFrom[u->index] = v->index;
                
                for (int t = 0; t < 4; t++)
                {
                    int targetIndex = targetIndices[t];
                    if (u->index == targetIndex)
                    {
                        reachedTargetIndex = u->index;
                        break;
                    }
                }
                
                queueCount++;
            }
            
            if (reachedTargetIndex >= 0)
            {
                break;
            }
        }
        
        if (reachedTargetIndex >= 0)
        {
            break;
        }
        
        queueCount--;
        queueIndex++;
    }
    
    int result = -1;
    if (reachedTargetIndex >= 0)
    {
        int hopCount = 0;
        int nextHopIndex = reachedTargetIndex;
        int kingPosIndex = this->kingPos.y * DIM + this->kingPos.x;
        
        while (nextHopIndex != kingPosIndex)
        {
            hopCount++;
            nextHopIndex = visitedFrom[nextHopIndex];
        }
        
        result = hopCount;
    }
    
    return result;
}

int GameState::calcTokenDiff()
{
    int result = 0;
    
    for (int16_t y = 0; y < DIM; y++)
    {
        for (int16_t x = 0; x < DIM; x++)
        {
            Vector2 p = {x, y};
            
            if (this->hasStateAtPos(p, FIELD_WHITE))
            {
                result--;
            }
            else if (this->hasStateAtPos(p, FIELD_BLACK))
            {
                result ++;
            }
        }
    }
    
    return result;
}

int GameState::calcKingFreedom()
{
    int result = 0;
    
    Vector2 kingPos = this->kingPos;
    for (int i = 0; i < 4; i++)
    {
        Vector2 dir = DIRECTIONS[i];
        Vector2 p = add(dir, kingPos);
        
        if (this->hasStateAtPos(p, FIELD_BLACK))
        {
            result++;
        }
    }
    
    return result;
}
