#include "Snake.h"
#include <Windows.h>

SnakeGame::SnakeGame(void (*render)(SnakeGame*))
{
	_render = render;
	_playerOne = Player(Point(PLAYER_INITIAL_LENGTH+1, PLAYER_INITIAL_LENGTH+1), DIR_RIGHT);
}

SnakeGame::~SnakeGame(void)
{
}

void SnakeGame::Run()
{
	while(true)
	{
		Logic();
		Render();
		Sleep(100); // TODO Windows-only
	}
}

void SnakeGame::Logic()
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

void SnakeGame::Render() {
	_render(this);
}

Player::Player(Point spawnPoint, Direction dir)
	: _head(spawnPoint)
{
	_segments[0] = dir | PLAYER_INITIAL_LENGTH;
	_segments[1] = 0; // Zero byte signifies end of snake
	// Don't care about the rest of it, quite honestly.
}

Player::Player(void)
	: _head(0, 0)
{ }

Player::~Player(void)
{ }

Point Player::GetNextCoord() {
	Point nextCoord = _head;
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
	return _segments[0] & DIR_MASK;
}

bool Player::CollidedBy(const Point& coord) {
	// The previous (headward) segment ended at this coordinate.
	Point lastSegmentEnd = _head;

	for (int i = 0; i < PLAYER_MAX_SEGMENTS; i++) {
		// The direction the snake is facing
		Direction dir = _segments[i] & DIR_MASK;
		uint8_t length = _segments[i] & LENGTH_MASK;

		// If we hit a zero-length segment, we have reached the snake's tail.
		if (length == 0) return false;

		// Does this segment travel vertically?
		if (dir & DIR_VERTICAL_MASK != 0) {
			// Vertical travel means X coordinate must match.
			if (coord.x == lastSegmentEnd.x) {
				if (dir & DIR_DOWNRIGHT_MASK == 0) {
					// Snake is going up, so the lastSegmentEnd has the lowest Y-value and the highest is (lastSegmentEnd.y + length)
					if (lastSegmentEnd.y <= coord.y && coord.y <= lastSegmentEnd.y + length)
						return true;

					lastSegmentEnd.y += length;
				} else {
					if (lastSegmentEnd.y - length <= coord.y && coord.y <= lastSegmentEnd.y)
						return true;

					lastSegmentEnd.y -= length;
				}
			}
		} else {
			// Horizontal travel means Y coordinate must match.
			if (coord.y == lastSegmentEnd.y) {
				if (dir & DIR_DOWNRIGHT_MASK == 0) {
					// Snake is going left, so the lastSegmentEnd has the lowest X-value and the highest is (lastSegmentEnd.x + length)
					if (lastSegmentEnd.x <= coord.x && coord.x <= lastSegmentEnd.x + length)
						return true;

					lastSegmentEnd.x += length;
				} else {
					if (lastSegmentEnd.x - length <= coord.x && coord.x <= lastSegmentEnd.x)
						return true;

					lastSegmentEnd.x -= length;
				}
			}
		}
	}

	// We reached the end of the array without a zero-length segment *or* a collision? Ho, boy.
	return false;
}