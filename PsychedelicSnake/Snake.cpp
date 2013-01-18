#include "Snake.h"
#include <Windows.h>

Snake::Snake(uint8_t width, uint8_t height, void (*render)(uint8_t*))
{
	_width = width;
	_height = height;
	_render = render;
	_playerOne = Player(Point(PLAYER_INITIAL_LENGTH+1, PLAYER_INITIAL_LENGTH+1), DIR_RIGHT);
}

Snake::~Snake(void)
{
}

void Snake::Run()
{
	uint8_t world[32];
	while(true)
	{
		Tick();
		_render(world);
		Sleep(100); // TODO convert to AVR
	}
}

void Snake::Tick()
{	
	// Apply Player 1's controls

	// Check for Player 1 collision
	//	Determine the coordinate Player 1's head will move into
	Point nextCoord = _playerOne.GetNextCoord();
	//	IF coordinate is a boundary
	if (nextCoord.x == WORLD_RBOUND
		|| nextCoord.x == 0
		|| nextCoord.y == WORLD_BBOUND
		|| nextCoord.y == 0
		//	 OR coordinate intersects Player 1's body
		|| _playerOne.CollidedBy(nextCoord))
	{
		_victoryState = VICTORY_P2_WIN; // Game over

		// TODO: Handle P2
		// TODO: Handle tie
	}

	// Check for Player 1 bonus
	//	IF
	//		coordinate is an apple
	//	THEN
	//		Add length to Player 1

	// Move Player 1 forward

}

Player::Player(Point spawnPoint, Direction dir)
	: head(spawnPoint)
{
	segments[0] = dir | PLAYER_INITIAL_LENGTH;
	segments[1] = 0; // Zero byte signifies end of snake
}

Player::Player(void)
	: head(0, 0)
{ }

Player::~Player(void)
{ }

Point Player::GetNextCoord() {
	Point nextCoord = head;
	switch (GetDirection())
	{
	case DIR_RIGHT:
		nextCoord.x++;
		break;
	case DIR_UP:
		nextCoord.y--;
		break;
	case DIR_LEFT:
		nextCoord.x--;
		break;
	case DIR_DOWN:
		nextCoord.y++;
		break;
	}

	return nextCoord;
}

Direction Player::GetDirection() {
	return segments[0] & DIR_MASK;
}

bool Player::CollidedBy(const Point& coord) {

	Point lastSegmentEnd = head;

	for (int i = 0; i < PLAYER_MAX_SEGMENTS; i++)
	{
		// The direction the snake is facing
		Direction dir = segments[i] & DIR_MASK;
		uint8_t length = segments[i] & LENGTH_MASK;

		// Does this segment travel vertically?
		if (dir & DIR_VERTICAL_MASK != 0)
		{
			// Vertical travel means X coordinate must match.
			if (coord.x != lastSegmentEnd.x)
				continue;

			uint8_t yFinal = (dir & DIR_DOWNLEFT_MASK == 0)		// If == 0, snake is going up
								? lastSegmentEnd.y + length	// Backtrack down
								: lastSegmentEnd.y - length;	// Backtrack up
			
			
		}
	}

	return false;
}