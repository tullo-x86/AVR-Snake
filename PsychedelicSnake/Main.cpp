#include "Snake.h"

void render(SnakeGame*);

void main()
{
	SnakeGame game = SnakeGame(render);
}

void render(SnakeGame* game) {
	// Go screw with PixelToaster's buffer and tell it to render
	// or whatever it is the kids do these days
}