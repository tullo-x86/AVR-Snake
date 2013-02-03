#include "Snake.h"
#include "PixelToaster.h"
#include <Windows.h>

using namespace PixelToaster;

void setPixel(uint8_t, uint8_t, uint8_t);
void clearFrameBuffer();

const int ScaleFactor = 20;
const int ScreenWidth = WORLD_WIDTH * ScaleFactor;
const int ScreenHeight = WORLD_HEIGHT * ScaleFactor;

Pixel palette[256];
vector<Pixel> pixels;

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

	Display display( "Fullscreen Example", 600, 600, Output::Windowed );
	pixels = vector<Pixel>( ScreenWidth * ScreenHeight);

	SnakeGame game = SnakeGame(setPixel, clearFrameBuffer);
	while ( display.open() )
	{
		/*
		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int x = 0; x < WORLD_WIDTH; x++)
			{
				setPixel(x, y, (x + y*WORLD_WIDTH) % 256);
			}
		}
		*/
		Sleep(100);
		game.Tick();

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
