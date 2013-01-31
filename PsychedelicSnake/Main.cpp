#include "Snake.h"
#include "PixelToaster.h"

using namespace PixelToaster;

void render(SnakeGame*);
void setPixel(int, int, uint8_t);

const int ScaleFactor = 20;
const int ScreenWidth = WORLD_WIDTH * ScaleFactor;
const int ScreenHeight = WORLD_HEIGHT * ScaleFactor;

Pixel palette[256];
vector<Pixel> pixels;

void main()
{
	for (int i = 0; i < 256; i++)
	{
		palette[i].r = (i % 64) / 63.0f;
		palette[i].g = (i % 128) / 127.0f;
		palette[i].b = i / 255.0f;
	}
	Display display( "Fullscreen Example", 600, 600, Output::Windowed );
	pixels = vector<Pixel>( ScreenWidth * ScreenHeight);
	
	SnakeGame game = SnakeGame(render);
    while ( display.open() )
    {
		/*
        unsigned int index = 0;
        for ( int y = 0; y < ScreenHeight; ++y )
        {
            for ( int x = 0; x < ScreenWidth; ++x )
            {
                pixels[index].r = 0.8f + y * 0.0015f;
                pixels[index].g = 0.2f + y * 0.00075f;
                pixels[index].b = 0.1f + y * 0.0005f;

				++index;
            }
        }*/

		for (int y = 0; y < WORLD_HEIGHT; y++)
		{
			for (int x = 0; x < WORLD_WIDTH; x++)
			{
				setPixel(x, y, (x + y*WORLD_WIDTH) % 256);
			}
		}

		display.update( pixels );
    }
}

void render(SnakeGame* game) {
	// Go screw with PixelToaster's buffer and tell it to render
	// or whatever it is the kids do these days


}

void setPixel(int worldX, int worldY, uint8_t colour) {
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