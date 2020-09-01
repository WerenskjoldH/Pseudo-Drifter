#ifndef GFX_HELPER_H
#define GFX_HELPER_H

/*
	Description: This helper file is used for handling all drawing functions in the game
					By consolidating all these functions we can quickly make sweeping changes to the rendering process should a better/faster approach be desired

	To-do: 
		* Support for rendering sprites
*/

#include <SDL.h>

static int maxLength = 0;

// Don't use this standalone
inline void GfxDrawCircle(SDL_Renderer* renderer, int cx, int cy, int x, int y)
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
inline void GfxDrawFilledCircle(SDL_Renderer* renderer, int cx, int cy, int x, int y)
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
/* Draws a circle using the Bresenham Circle Algorithm
	@param SDL_Renderer Render target
	@param cx X-axis component of the circle's center
	@param cy Y-axis component of the circle's center
	@oaram radius Radius of the circle
	@param filled If the circle should be filled or not
*/
inline void GfxDrawBrenCircle(SDL_Renderer* renderer, int cx, int cy, int radius, bool filled)
{
	if (radius <= 1)
	{
		SDL_RenderDrawPoint(renderer, cx, cy);
		return;
	}

	int x = 0, y = radius, d = 3 - (2 * radius);

	if (!filled)
		GfxDrawCircle(renderer, cx, cy, x, y);
	else
		GfxDrawFilledCircle(renderer, cx, cy, x, y);

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
			GfxDrawCircle(renderer, cx, cy, x, y);
		else
			GfxDrawFilledCircle(renderer, cx, cy, x, y);
	}
}

/* Draws an "endless" line to the renderer
	@param SDL_Renderer Render target
	@param cx X-axis component of the point where the line will rotate
	@param cy Y-axis component of the point where the line will rotate
	@param sw Screen width
	@param sh Screen height
	@param angle Angle that the line is aimed at ( in radians )
*/
inline void GfxDrawEndlessLine(SDL_Renderer* renderer, int cx, int cy, int sw, int sh, float angle)
{
	if (maxLength == 0)
	{
		// :^)
		maxLength = sh / sinf(atan(sh / sw));
	}

	int posX = int(cos(angle) * 2 * sw);
	int posY = int(sin(angle) * 2 * sh);
	
	SDL_RenderDrawLine(renderer, cx + posX, cy + posY, cx - posX, cy - posY);
}

inline void GfxDrawHorizontalLine(SDL_Renderer* renderer, int x, int y, int width)
{
	SDL_RenderDrawLine(renderer, x - width, y, x + width, y);
}

#endif