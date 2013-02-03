#include "Snake.h"
#include "PixelToaster.h"
#include <Windows.h>
#include <ctime>

using namespace PixelToaster;

void setPixel(uint8_t, uint8_t, uint8_t);
void clearFrameBuffer();

const int ScaleFactor = 20;
const int ScreenWidth = WORLD_WIDTH * ScaleFactor;
const int ScreenHeight = WORLD_HEIGHT * ScaleFactor;

Pixel palette[256];
vector<Pixel> pixels;

Direction player1Control = DIR_RIGHT;

class SnakeListener : public Listener {
public:
	void onKeyDown(DisplayInterface& display, Key key) {
		switch (key)
		{
		case Key::Right:
		case Key::D:
			player1Control = DIR_RIGHT;
			break;
		case Key::Left:
		case Key::A:
			player1Control = DIR_LEFT;
			break;
		case Key::Up:
		case Key::W:
			player1Control = DIR_UP;
			break;
		case Key::Down:
		case Key::S:
			player1Control = DIR_DOWN;
			break;
		default:
			break;
		}
	}
};


void main()
{
	for (int i = 0; i < 64; i++)
	{
		palette[i].r = i / 63.0f;
		palette[i].g = palette[i].b = 0;
	}

	for (int i = 0; i < 64; i++)
	{
		palette[i+64].g = i / 63.0f;
		palette[i+64].r = palette[i].b = 0;
	}

	for (int i = 0; i < 64; i++)
	{
		palette[i+128].b = i / 63.0f;
		palette[i+128].r = palette[i].g = 0;
	}

	for (int i = 0; i < 64; i++)
	{
		palette[i+192].r = palette[i+192].g = palette[i+192].b = i / 63.0f;
	}

	Display display( "Snake test", ScreenWidth, ScreenHeight, Output::Windowed );
	pixels = vector<Pixel>( ScreenWidth * ScreenHeight);

	display.listener(new SnakeListener());

	clock_t time = clock();
	clock_t interval = CLOCKS_PER_SEC / 10;
	clock_t nextTick = time + interval;

	SnakeGame game = SnakeGame(setPixel, clearFrameBuffer);
	while ( display.open() )
	{
		game.ApplyPlayer1Control(player1Control);
		time = clock();
		if (time > nextTick)
		{
			nextTick = time + interval;
			clearFrameBuffer();
			game.Tick();
		}
		display.update( pixels );
	}
}

void clearFrameBuffer() {
	for (int y = 0; y < WORLD_HEIGHT; y++)
	{
		for (int x = 0; x < WORLD_WIDTH; x++)
		{
			setPixel(x, y, 0);
		}
	}
}

void setPixel(uint8_t worldX, uint8_t worldY, uint8_t colour) {
	const int minX = worldX * ScaleFactor;
	const int minY = worldY * ScaleFactor;
	const int maxX = (worldX+1) * ScaleFactor - 1;
	const int maxY = (worldY+1) * ScaleFactor - 1;

	for (int screenX = minX; screenX <= maxX; screenX++)
	{
		for (int screenY = minY; screenY <= maxY; screenY++)
		{
			pixels[screenX + (screenY * ScreenWidth)] = palette[colour];
		}
	}
}
