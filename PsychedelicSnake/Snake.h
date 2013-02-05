#pragma once
#include <stdint.h>

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

// Long enough to zig-zag across a 30x30 grid.
// If a player does this then they are being an idiot and will lose segments.
#define PLAYER_MAX_SEGMENTS (WORLD_WIDTH + WORLD_HEIGHT)

#define PLAYER_INITIAL_LENGTH 3

struct GameState
{
	enum Enumeration {
		Attract,
		Playing,
		Player1Win,
		Player2Win,
		Tie
	};
};

typedef uint8_t SnakeSegment;
typedef void(*DrawPixel)(uint8_t x, uint8_t y, uint8_t colour);
typedef void(*ClearFrameBuffer)();
typedef uint8_t byte;

#define COL_P1_HEAD 63
#define COL_P1_BODY 32

class Player
{
public:
	Player();
	Player(Point spawnPoint, Direction dir);
	~Player();

	Point GetNextCoord();
	bool CollidedBy(const Point& coord);

	void Draw(DrawPixel);

	void ApplyControl(Direction dir);
	void MoveForward();

private:
	Point _head;
	SnakeSegment _segments[PLAYER_MAX_SEGMENTS];
	Direction _facing;
};

class SnakeGame
{
public:
	SnakeGame(DrawPixel, ClearFrameBuffer);
	~SnakeGame(void);

	void ApplyPlayer1Control(Direction dir);

	void Tick();
private:
	void (*_renderCoord)(uint8_t x, uint8_t y, uint8_t colour);
	void (*_clearFrameBuffer)();

	void StartGame();

	void Logic();

	void PlayingLogic();

	void Render();

	Player _playerOne;

	GameState::Enumeration _gameState;
};

