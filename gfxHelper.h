#ifndef GFX_HELPER_H
#define GFX_HELPER_H

#include <SDL/SDL.h>

#define PI  3.14159265359

// Don't use this standalone
void gfxDrawCircle(SDL_Renderer* renderer, int cx, int cy, int x, int y)
{
	SDL_RenderDrawPoint(renderer, cx + x, cy + y);
	SDL_RenderDrawPoint(renderer, cx - x, cy + y);
	SDL_RenderDrawPoint(renderer, cx + x, cy - y);
	SDL_RenderDrawPoint(renderer, cx - x, cy - y);
	SDL_RenderDrawPoint(renderer, cx + y, cy + x);
	SDL_RenderDrawPoint(renderer, cx - y, cy + x);
	SDL_RenderDrawPoint(renderer, cx + y, cy - x);
	SDL_RenderDrawPoint(renderer, cx - y, cy - x);
}

// Don't use this standalone
void gfxDrawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int x, int y)
{
	for (int i = -x; i < x; i++)
	{
		SDL_RenderDrawPoint(renderer, cx + i, cy + y);
		SDL_RenderDrawPoint(renderer, cx + i, cy - y);
	}

	for (int j = -y; j < y; j++)
	{
		SDL_RenderDrawPoint(renderer, cx + j, cy + x);
		SDL_RenderDrawPoint(renderer, cx + j, cy - x);
	}
}

// Bresenham's Circle Algorithm
void gfxDrawBrenCircle(SDL_Renderer* renderer, int cx, int cy, int radius, bool filled)
{
	if (radius <= 1)
	{
		SDL_RenderDrawPoint(renderer, cx, cy);
		return;
	}

	int x = 0, y = radius, d = 3 - (2 * radius);

	if (!filled)
		gfxDrawCircle(renderer, cx, cy, x, y);
	else
		gfxDrawFilledCircle(renderer, cx, cy, x, y);

	while (x <= y)
	{
		x++;
		
		if (d < 0)
			d = d + (4 * x) + 6;
		else
		{
			d = d + 4 * (x - y) + 6;
			y--;
		}


		if (!filled)
			gfxDrawCircle(renderer, cx, cy, x, y);
		else
			gfxDrawFilledCircle(renderer, cx, cy, x, y);
	}
}

/* Parameters:
	SDL_Renderer: Render target
	cx			: The x-axis component of the point where the line will rotate
	cy			: The y-axis component of the point where the line will rotate
	sw			: The screen width
	sh			: The screen height
	angle		: The angle that the line is aimed at ( in radians )
*/
static int maxLength = 0;
void gfxDrawEndlessLine(SDL_Renderer* renderer, int cx, int cy, int sw, int sh, float angle)
{
	if (maxLength == 0)
	{
		// :^)
		maxLength = sh / sinf(atan(sh / sw));
	}

	int posX = cos(angle) * 2 * sw;
	int posY = sin(angle) * 2 * sh;
	
	SDL_RenderDrawLine(renderer, cx + posX, cy + posY, cx - posX, cy - posY);
}

void gfxDrawHorizontalLine(SDL_Renderer* renderer, int x, int y, int width)
{
	SDL_RenderDrawLine(renderer, x - width, y, x + width, y);
}

#endif