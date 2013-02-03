#include "Snake.h"

SnakeGame::SnakeGame(
	DrawPixel drawPixel,
	ClearFrameBuffer clear)
{
	_renderCoord = drawPixel;
	_clearFrameBuffer = clear;
	_playerOne = Player(Point(PLAYER_INITIAL_LENGTH+1, PLAYER_INITIAL_LENGTH+1), DIR_RIGHT);
}

SnakeGame::~SnakeGame(void)
{
}

void SnakeGame::Tick()
{
	Logic();
	Render();
}

void SnakeGame::Logic()
{
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
		// TODO: Handle tie (both snakes collided, or both moving into same coordinate
	}

	// Check for Player 1 bonus
	//	IF
	//		coordinate is an apple
	//	THEN
	//		tell Player 1 to grow

	_playerOne.MoveForward();
}

void SnakeGame::ApplyPlayer1Control(Direction dir) {
	_playerOne.ApplyControl(dir);
}

void SnakeGame::Render() {
	_clearFrameBuffer();

	_playerOne.Draw(_renderCoord);
}

Player::Player(Point spawnPoint, Direction dir)
	: _head(spawnPoint)
{
	_facing = dir;
	_segments[0] = dir | PLAYER_INITIAL_LENGTH;
	_segments[1] = DIR_UP | 6;
	_segments[2] = DIR_LEFT | 1;
	_segments[3] = DIR_UP | 3;
	_segments[4] = 0; // Null terminator

	// Don't need to initialise anything else since it'll get
	// clobbered as the snake moves.
}

Player::Player(void)
	: _head(0, 0)
{ }

Player::~Player(void)
{ }

Point Player::GetNextCoord() {
	Point nextCoord = _head;
	switch (_facing)
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
		if ((dir & DIR_VERTICAL_MASK) != 0) {
			// Vertical travel means X coordinate must match.
			if (coord.x == lastSegmentEnd.x) {
				if ((dir & DIR_DOWNRIGHT_MASK) == 0) {
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
				if ((dir & DIR_DOWNRIGHT_MASK) == 0) {
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

void Player::ApplyControl(Direction dir) {
	byte wasVert = _segments[0] & DIR_VERTICAL_MASK;
	byte isVert = dir & DIR_VERTICAL_MASK;
	if(wasVert != isVert) {
		_facing = dir;
	}
}

void NudgeTailwards(Point& point, Direction direction) {
	if (direction == DIR_RIGHT)	{
		point.x--;
	}
	else if (direction == DIR_LEFT) {
		point.x++;
	}
	else if (direction == DIR_UP) {
		point.y++;
	}
	else {
		point.y--;
	}

}

void Player::Draw(DrawPixel draw){

	// Head pixel is a special colour
	draw(_head.x, _head.y, COL_P1_HEAD);

	Point coord = _head;

	byte bodyColour = COL_P1_BODY;
	for (int i = 0; i < PLAYER_MAX_SEGMENTS; i++) {
		// The direction the snake is facing
		Direction dir = _segments[i] & DIR_MASK;
		uint8_t length = _segments[i] & LENGTH_MASK;

		// If we hit a zero-length segment, we have reached the snake's tail.
		if (length == 0) return;

		for (int i = 0; i < length; i++)
		{
			NudgeTailwards(coord, dir);
			draw(coord.x, coord.y, bodyColour);
			bodyColour -= 2;
		}
	}
}

void Player::MoveForward() {
	_head = GetNextCoord();

	// Are we changing direction?
	if ((_segments[0] & DIR_MASK) != _facing)
	{
		// If this gets expensive I guess we can use an offset.
		// Constant time is nice and reliable however.
		for (int i = PLAYER_MAX_SEGMENTS - 1; i > 0; i--)
		{
			_segments[i] = _segments[i-1];
		}
		_segments[0] = _facing | 0x01;
	}
	else
	{
		_segments[0]++;
	}

	// Find the last segment. Yes, it could be the first one.
	byte lastSegmentIndex = 0;
	while (_segments[++lastSegmentIndex] & LENGTH_MASK);
	lastSegmentIndex--;

	_segments[lastSegmentIndex]--;
}
