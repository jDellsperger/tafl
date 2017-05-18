#include "Gamestate.h"
#include <limits>

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
    if (player == PLAYER_BLACK)
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
    if (player == PLAYER_BLACK)
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
    if (player == PLAYER_BLACK)
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
	Field f = this->fields[DIM][DIM];
	int16_t val = 0;
	Field* t;
	Vector2 vt;
	uint32_t d = UINT32_MAX;

	Vector2 tl = { 0, 0 };
	Vector2 tr = { DIM, 0 };
	Vector2 dl = { 0, DIM };
	Vector2 dr = { DIM, DIM };

	Vector2 k = this->kingPos;

	if (distanceSq(tl, k) < d)
	{
		d = distanceSq(tl, k);
		t = this->getFieldAtPos(tl);
		vt = tl;
	}
	
	if (distanceSq(tr, k) < d)
	{
		d = distanceSq(tr, k);
		t = this->getFieldAtPos(tr);
		vt = tr;
	}
	
	if (distanceSq(dl, k) < d)
	{
		d = distanceSq(dl, k);
		t = this->getFieldAtPos(dl);
		vt = dl;
	}
	
	if (distanceSq(dr, k) < d)
	{
		d = distanceSq(dr, k);
		t = this->getFieldAtPos(dr);
		vt = dr;
	}

	for (int y = min(k.y, vt.y); y < max(k.y, vt.y); y++)
	{
		for (int x = min(k.x, vt.x); x < max(k.x, vt.x); x++)
		{
			Field f = this->fields[x][y];

			if (f.hasFlags(WHITE)) 
			{
				val--;
			}
			else if (f.hasFlags(BLACK))
			{
				val = val + 2;
			}
		}
	}

	return val;
}