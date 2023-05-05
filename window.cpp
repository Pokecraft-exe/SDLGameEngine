#include "window.h"

window::window() {
	Window = nullptr;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}
	else {
		Window = SDL_CreateWindow("default Empty", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		screenSurface = SDL_GetWindowSurface(Window);
		if (Window == nullptr) {
			return;
		}
		else {
			SDL_UpdateWindowSurface(Window);
			bool quit = false;
			created = 1;
		}
	}
}

window::window(const char* name) {
	w_name = name;

	Window = nullptr;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}
	else {
		Window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		screenSurface = SDL_GetWindowSurface(Window);
		if (Window == nullptr) {
			return;
		}
		else {
			SDL_UpdateWindowSurface(Window);
			bool quit = false;
			created = 1;
		}
	}
}

window::window(const char* name, unsigned int w, unsigned int h) {

	w_name = name;

	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;

	Window = nullptr;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}
	else {
		Window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		screenSurface = SDL_GetWindowSurface(Window);
		if (Window == nullptr) {
			return;
		}
		else {
			SDL_UpdateWindowSurface(Window);
			bool quit = false;
			created = 1;
		}
	}
}

void window::Clip(unsigned int& x, unsigned int& y)
{
	if (x < 1) x = 1;
	if (x >= SCREEN_WIDTH - 1) { x = SCREEN_WIDTH - 1; }
	if (y < 1) y = 1;
	if (y >= SCREEN_HEIGHT - 1) { y = SCREEN_HEIGHT - 1; }
}

void window::Clip(int& x, int& y)
{
	if (x <= 1) x = 1;
	if (x >= SCREEN_WIDTH - 1) { x = SCREEN_WIDTH - 1; }
	if (y <= 1) y = 1;
	if (y >= SCREEN_HEIGHT - 1) { y = SCREEN_HEIGHT - 1; }
}

void window::putPixel(unsigned int x, unsigned int y, uint32_t color) {
	if (x < 1) return;
	if (x >= SCREEN_WIDTH - 1) { x = SCREEN_WIDTH - 1; }
	if (y < 1) return;
	if (y >= SCREEN_HEIGHT - 1) { y = SCREEN_HEIGHT - 1; }
	Uint8* p = (Uint8*)screenSurface->pixels + (y * screenSurface->pitch + x * 4);
	*(uint32_t*)p = color;//.Ablend(getPixel(x, y), color);
}

Color window::getPixel(unsigned int x, unsigned int y) {
	return *(Uint32*)((Uint8*)screenSurface->pixels + y * screenSurface->pitch + x * 4);
}

void window::update() {
	SDL_UpdateWindowSurface(Window);
}

void window::changeSize(unsigned int w, unsigned int h) {
	SDL_DestroyWindow(Window);
	SDL_Quit();

	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;

	Window = nullptr;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}
	else {
		Window = SDL_CreateWindow(w_name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		screenSurface = SDL_GetWindowSurface(Window);
		if (Window == nullptr) {
			return;
		}
		else {
			SDL_UpdateWindowSurface(Window);
			bool quit = false;
			created = 1;
		}
	}
}

void window::changeName(std::string name) {
	SDL_DestroyWindow(Window);
	SDL_Quit();

	w_name = name;

	Window = nullptr;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}
	else {
		Window = SDL_CreateWindow(w_name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		screenSurface = SDL_GetWindowSurface(Window);
		if (Window == nullptr) {
			return;
		}
		else {
			SDL_UpdateWindowSurface(Window);
			bool quit = false;
			created = 1;
		}
	}
}

void window::clear() {
	size_t buffer_size = SCREEN_HEIGHT * SCREEN_WIDTH * 4;
	for (uint64_t i = 0; i < buffer_size; i+=4) { 
		Uint8* p = (Uint8*)screenSurface->pixels + i;
		*(Uint32*)p = (uint32_t)0x00000000;
	}
}

void window::drawLine(int x0, int y0, int x1, int y1, uint32_t color) {
	int dx, dy, p, x, y;

	dx = x1 - x0;
	dy = y1 - y0;

	x = x0;
	y = y0;

	p = 2 * dy - dx;

	while (x < x1)
	{
		if (p >= 0)
		{
			putPixel(x, y, color);
			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			putPixel(x, y, color);
			p = p + 2 * dy;
		}
		x = x + 1;
	}
}

void window::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x3, y3, color);
	drawLine(x3, y3, x1, y1, color);
}

void window::drawAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
	drawAALine(x1, y1, x2, y2, color);
	drawAALine(x2, y2, x3, y3, color);
	drawAALine(x3, y3, x1, y1, color);
}

void window::quit() {
	SDL_DestroyWindow(Window);
	SDL_Quit();
	created = 0;
}

uint8_t blendColorValue(uint8_t a, uint8_t b, uint8_t t) { return ((a + b) / ((128*2)/t)); }

uint32_t Color::Ablend(Color fg, Color bg) {
	Color c;
	uint8_t iA = (255 - fg.A());
	if (fg.A() == 0) { return bg; }
	c.A(fg.A() + bg.A() * (255 - fg.A()));
	c.R((fg.R() * fg.A() + bg.R() * bg.A() * iA) / c.A());
	c.G((fg.G() * fg.A() + bg.G() * bg.A() * iA) / c.A());
	c.B((fg.B() * fg.A() + bg.B() * bg.A() * iA) / c.A());
	return c;
}

void SWAP(int& x, int& y) { int t = x; x = y; y = t; };

void window::drawAALine(int x0, int y0, int x1, int y1, Color c)
{
	int steep = absolute(y1 - y0) > absolute(x1 - x0);

	// swap the co-ordinates if slope > 1 or we
	// draw backwards
	if (steep)
	{
		SWAP(x0, y0);
		SWAP(x1, y1);
	}
	if (x0 > x1)
	{
		SWAP(x0, x1);
		SWAP(y0, y1);
	}

	//compute the slope
	float dx = x1 - x0;
	float dy = y1 - y0;
	float gradient = dy / dx;
	if (dx == 0.0)
		gradient = 1;

	int xpxl1 = x0;
	int xpxl2 = x1;
	float intersectY = y0;


	if (steep)
	{
		int x;
		for (x = xpxl1; x <= xpxl2; x++)
		{
			Color color = c;

			color.A(rfPartOfNumber(intersectY));

			putPixel(iPartOfNumber(intersectY), x, color);

			color.A(fPartOfNumber(intersectY));

			putPixel(iPartOfNumber(intersectY) - 1, x, color);
			intersectY += gradient;
		}
	}
	else
	{
		int x;
		for (x = xpxl1; x <= xpxl2; x++)
		{
			Color color = c;

			color.A(rfPartOfNumber(intersectY));

			putPixel(x, iPartOfNumber(intersectY), color);

			color.A(fPartOfNumber(intersectY));

			putPixel(x, iPartOfNumber(intersectY) - 1, color);
			intersectY += gradient;
		}
	}

}

void window::fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
	
	/*

		A big thanks to OneLoneCoder/Javidx9 for this precious code !!! =)
		link to the code it's from: https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/olcConsoleGameEngine.h
		his GitHub : https://github.com/OneLoneCoder

		He also inspierd me to do this Game engine, so a big thanks to him an his work!

	*/

	auto drawline = [&](int sx, int ex, int ny) { for (int i = sx; i <= ex; i++) putPixel(i, ny, color); };

	int t1x, t2x, y, minx, maxx, t1xp, t2xp;
	bool changed1 = false;
	bool changed2 = false;
	int signx1, signx2, dx1, dy1, dx2, dy2;
	int e1, e2;
	// Sort vertices
	if (y1 > y2) { SWAP(y1, y2); SWAP(x1, x2); }
	if (y1 > y3) { SWAP(y1, y3); SWAP(x1, x3); }
	if (y2 > y3) { SWAP(y2, y3); SWAP(x2, x3); }

	t1x = t2x = x1; y = y1;   // Starting points
	dx1 = (int)(x2 - x1); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y2 - y1);

	dx2 = (int)(x3 - x1); if (dx2 < 0) { dx2 = -dx2; signx2 = -1; }
	else signx2 = 1;
	dy2 = (int)(y3 - y1);

	if (dy1 > dx1) {   // swap values
		SWAP(dx1, dy1);
		changed1 = true;
	}
	if (dy2 > dx2) {   // swap values
		SWAP(dy2, dx2);
		changed2 = true;
	}

	e2 = (int)(dx2 >> 1);
	// Flat top, just process the second half
	if (y1 == y2) goto next;
	e1 = (int)(dx1 >> 1);

	for (int i = 0; i < dx1;) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			i++;
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) t1xp = signx1;//t1x += signx1;
				else          goto next1;
			}
			if (changed1) break;
			else t1x += signx1;
		}
		// Move line
	next1:
		// process second line until y value is about to change
		while (1) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;//t2x += signx2;
				else          goto next2;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next2:
		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);    // Draw line from min to max points found on the y
		// Now increase y
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y == y2) break;

	}
next:
	// Second half
	dx1 = (int)(x3 - x2); if (dx1 < 0) { dx1 = -dx1; signx1 = -1; }
	else signx1 = 1;
	dy1 = (int)(y3 - y2);
	t1x = x2;

	if (dy1 > dx1) {   // swap values
		SWAP(dy1, dx1);
		changed1 = true;
	}
	else changed1 = false;

	e1 = (int)(dx1 >> 1);

	for (int i = 0; i <= dx1; i++) {
		t1xp = 0; t2xp = 0;
		if (t1x < t2x) { minx = t1x; maxx = t2x; }
		else { minx = t2x; maxx = t1x; }
		// process first line until y value is about to change
		while (i < dx1) {
			e1 += dy1;
			while (e1 >= dx1) {
				e1 -= dx1;
				if (changed1) { t1xp = signx1; break; }//t1x += signx1;
				else          goto next3;
			}
			if (changed1) break;
			else   	   	  t1x += signx1;
			if (i < dx1) i++;
		}
	next3:
		// process second line until y value is about to change
		while (t2x != x3) {
			e2 += dy2;
			while (e2 >= dx2) {
				e2 -= dx2;
				if (changed2) t2xp = signx2;
				else          goto next4;
			}
			if (changed2)     break;
			else              t2x += signx2;
		}
	next4:

		if (minx > t1x) minx = t1x; if (minx > t2x) minx = t2x;
		if (maxx < t1x) maxx = t1x; if (maxx < t2x) maxx = t2x;
		drawline(minx, maxx, y);
		if (!changed1) t1x += signx1;
		t1x += t1xp;
		if (!changed2) t2x += signx2;
		t2x += t2xp;
		y += 1;
		if (y > y3) return;
	}
}

void window::fillAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {

	fillTriangle(x1, y1, x2, y2, x3, y3, color);
	drawAATriangle(x1, y1, x2, y2, x3, y3, 0xFF6666);
}

void window::setIcon(std::string path) {
	Icon = SDL_LoadBMP(path.data());
	SDL_SetWindowIcon(Window, Icon);
}

uint8_t Color::A() {
	return data[0];
}
uint8_t Color::B() {
	return data[1];
}
uint8_t Color::G() {
	return data[2];
}
uint8_t Color::R() {
	return data[3];
}

Color::Color(uint32_t color) { memcpy(&data, &color, sizeof(uint32_t)); }
Color::Color(Color& color) { uint32_t c = color; memcpy(&data, &color, sizeof(uint32_t)); }

Color::operator uint32_t() {
	uint32_t c;
	memcpy(&c, &data, sizeof(uint32_t));
	return c;
}