#pragma once
#include <stdint.h>

struct Pixel
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

struct Point
{
	Point(uint8_t xPos, uint8_t yPos)
	{
		x = xPos;
		y = yPos;
	}
	uint8_t x;
	uint8_t y;
};

// Consider direction as pointing from the tail to the head.
// In other words, the direction the player is facing.
#define DIR_VERTICAL_MASK	(0x01 << 6)
#define DIR_DOWNRIGHT_MASK	(0x02 << 6)
#define DIR_MASK			(0x03 << 6)

#define DIR_LEFT			(0x00 << 6)
#define DIR_UP				(0x01 << 6)
#define DIR_RIGHT			(0x02 << 6)
#define DIR_DOWN			(0x03 << 6)
#define LENGTH_MASK			(~DIR_MASK)

typedef uint8_t Direction;

#define WORLD_WIDTH 30
#define WORLD_HEIGHT 30
// Right boundary
#define WORLD_RBOUND (WORLD_WIDTH - 1)
// Bottom boundary
#define WORLD_BBOUND (WORLD_HEIGHT - 1)

// Zig-zag across a 30x30 grid. If a player does this then they are being an idiot and will lose segments.
#define PLAYER_MAX_SEGMENTS (WORLD_WIDTH + WORLD_HEIGHT)

#define PLAYER_INITIAL_LENGTH 3

#define VICTORY_FALSE	0
#define VICTORY_P1_WIN	1
#define VICTORY_P2_WIN	2
#define VICTORY_TIE		3

typedef uint8_t SnakeSegment;

class Player
{
public:
	Player();
	Player(Point spawnPoint, Direction dir);
	~Player();

	Point GetNextCoord();
	Direction GetDirection();
	bool CollidedBy(const Point& coord);

private:
	Point head;
	SnakeSegment segments[PLAYER_MAX_SEGMENTS];
};

class Snake
{
public:
	// Width and height represent the game world. Render accepts world data
	// in left-to-right, top-to-bottom order. World data excludes world
	// boundaries.
	Snake(uint8_t width, uint8_t height, void (*render)(uint8_t* worldData));
	~Snake(void);

	void Run();
private:
	uint8_t _width;
	uint8_t _height;
	uint8_t _maxPlayerLength;
	void (*_render)(uint8_t* worldData);

	Player _playerOne;

	void Tick();

	uint8_t _victoryState;
};

