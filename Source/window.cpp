#include "window.h"

window::window() {
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

	SCREEN_BASE_WIDTH = w;
	SCREEN_BASE_HEIGHT = h;

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

void window::resizable() {
	SDL_DestroyWindow(Window);
	SDL_Quit();

	Window = nullptr;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		return;
	}
	else {
		Window = SDL_CreateWindow(w_name.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
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

bool window::TestClip(int x, int y)
{
	if (x < 1 && 
		x >= SCREEN_WIDTH - 1 && 
		y < 1 && 
		y >= SCREEN_HEIGHT - 1 ) return true;
	return false;
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

void window::changeName(string name) {
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

void window::drawDepthLine(int x1, int y1, float w1, int x2, int y2, float w2, uint32_t c) {
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	float w, dw, dw1, pw, we = 0;
	dx = x2 - x1;
	dy = y2 - y1;
	dw = w2 - w1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	dw1 = fabs(dw);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	pw = 2 * dx1 - dw1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			w = w1;
			xe = x2;
			we = w2;
		}
		else
		{
			x = x2;
			y = y2;
			w = w2;
			xe = x1;
			we = w1;
		}
		if (w >= depthBuffer[y * SCREEN_WIDTH + x])
		{
			putPixel(x, y, c);
			depthBuffer[y * SCREEN_HEIGHT + x] = w;
		}
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			w = (x1 - x2) * w + (1 / (x1 - x2)) * we;
			if (px < 0)
			{
				px = px + 2 * dy1;
				pw = pw + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0 && dw < 0) || (dx > 0 && dy > 0 && dw > 0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
				pw = pw + 2 * (dy1 - dw1);
			}
			if (w >= depthBuffer[y * SCREEN_WIDTH + x])
			{
				putPixel(x, y, c);
				depthBuffer[y * SCREEN_HEIGHT + x] = w;
			}
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			w = w1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			w = w2;
			y = y2;
			ye = y1;
		}
		if (w >= depthBuffer[y * SCREEN_WIDTH + x])
		{
			putPixel(x, y, c);
			depthBuffer[y * SCREEN_HEIGHT + x] = w;
		}
		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x = x + 1;
					w = (x1 - x2) * w + (1 / (x1 - x2)) * we;
				}
				else
				{
					x = x - 1;
					w = (x1 + x2) / w - (1 / (x1 + x2)) / we;
				}
				py = py + 2 * (dx1 - dy1);
			}
			if (w >= depthBuffer[y * SCREEN_WIDTH + x])
			{
				putPixel(x, y, c);
				depthBuffer[y * SCREEN_HEIGHT + x] = w;
			}
		}
	}
}

void window::drawLine(int x1, int y1, int x2, int y2, uint32_t c)
{
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0)
		{
			x = x1;
			y = y1;
			xe = x2;
		}
		else
		{
			x = x2;
			y = y2;
			xe = x1;
		}
		putPixel(x, y, c);
		for (i = 0; x < xe; i++)
		{
			x = x + 1;
			if (px < 0)
			{
				px = px + 2 * dy1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y = y + 1;
				}
				else
				{
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			putPixel(x, y, c);
		}
	}
	else
	{
		if (dy >= 0)
		{
			x = x1;
			y = y1;
			ye = y2;
		}
		else
		{
			x = x2;
			y = y2;
			ye = y1;
		}
		putPixel(x, y, c);
		for (i = 0; y < ye; i++)
		{
			y = y + 1;
			if (py <= 0)
			{
				py = py + 2 * dx1;
			}
			else
			{
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x = x + 1;
				}
				else
				{
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			putPixel(x, y, c);
		}
	}
}

void window::fill(Color color) {
	size_t buffer_size = SCREEN_HEIGHT * SCREEN_WIDTH * 4;
	for (uint64_t i = 0; i < buffer_size; i += 4) {
		Uint8* p = (Uint8*)screenSurface->pixels + i;
		*(Uint32*)p = (uint32_t)color;
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

void window::fillPolygon(vector<vec3d> Points, uint32_t color) {
	if (Points.size() == 3) {
		fillTriangle(Points[0].x, Points[0].y, Points[1].x, Points[1].y, Points[2].x, Points[2].y, color);
		return;
	}
	int xmax = 0, ymax = 0, xmin = 0, ymin = 0, v = 0;
	vector<int> inter;
	v = Points.size();
	Points.push_back(Points[0]);
	float s, s2;

	for (vec3d& p : Points) {
		if (p.x > xmax) xmax = p.x;
		else if (xmin > p.x) xmin = p.x;
		if (p.y > ymax) ymax = p.y;
		else if (ymin > p.y) ymin = p.y;
		inter.push_back(0);
	}

	s = ymin + 0.01;
	while (s <= ymax)
	{
		int x1, x2, y1, y2, temp, c, x, y;
		c = 0;
		for (int i = 0; i < v; i++)
		{
			x1 = Points[i].x;
			y1 = Points[i].y;
			x2 = Points[i + 1].x;
			y2 = Points[i + 1].y;
			if (y2 < y1)
			{
				temp = x1;
				x1 = x2;
				x2 = temp;
				temp = y1;
				y1 = y2;
				y2 = temp;
			}
			if (s <= y2 && s >= y1)
			{
				if ((y1 - y2) == 0)
					x = x1;
				else // used to make changes in x. so that we can fill our polygon after cerain distance
				{
					x = ((x2 - x1) * (s - y1)) / (y2 - y1);
					x = x + x1;
				}
				if (x <= xmax && x >= xmin)
					inter[c++] = x;
			}
		}
		int j, i = 0;
		temp = 0;

		for (i = 0; i < v; i++)
		{
			drawLine(Points[i].x, Points[i].y, Points[i + 1].x, Points[i + 1].y, color); // used to make hollow outlines of a polygon
		}
		for (i = 0; i < c; i += 2)
		{
			drawLine(inter[i], s, inter[i + 1], s, color);  // Used to fill the polygon ....
		}
		s++;
	}
}

void window::fillDepthPolygon(vector<vec3d> Points, uint32_t color) {
	if (Points.size() < 3) return;
	int xmax = 0, ymax = 0, wmax = 0, xmin = 0, ymin = 0, wmin = 0, v = 0;
	vector<int> inter;
	vector<int> interW;
	v = Points.size();
	Points.push_back(Points[0]);
	float s, s2;

	for (vec3d& p : Points) {
		if (p.x > xmax) xmax = p.x;
		else if (xmin > p.x) xmin = p.x;
		if (p.y > ymax) ymax = p.y;
		else if (ymin > p.y) ymin = p.y;
		if (p.w > wmax) wmax = p.w;
		else if (wmin > p.w) ymin = p.w;
		inter.push_back(0);
		interW.push_back(0);
	}

	s = ymin + 0.01;
	while (s <= ymax)
	{
		int x1, x2, w1, w2, y1, y2, temp, c, c2, x, y, w;
		c = 0;
		c2 = 0;
		
		for (int i = 0; i < v; i++)
		{
			x1 = Points[i].x;
			y1 = Points[i].y;
			w1 = Points[i].w;
			x2 = Points[i + 1].x;
			y2 = Points[i + 1].y;
			w2 = Points[i + 1].w;
			if (y2 < y1)
			{
				temp = x1;
				x1 = x2;
				x2 = temp;
				temp = w1;
				w1 = w2;
				w2 = temp;
				temp = y1;
				y1 = y2;
				y2 = temp;
			}
			
			if (s <= y2 && s >= y1)
			{
				if ((y1 - y2) == 0){
					x = x1;
					w = w1;
				}
				else // used to make changes in x. so that we can fill our polygon after cerain distance
				{
					x = ((x2 - x1) * (s - y1)) / (y2 - y1);
					x = x + x1;
					w = ((w2 - w1) * (s - y1)) / (y2 - y1);
					w = w + w1;
				}
				if (x <= xmax && x >= xmin)
					inter[c++] = x;
				if (w <= wmax && w >= wmin)
					interW[c2++] = w;
			}
		}
		s++;
		int j, i = 0;
		temp = 0;

		for (i = 0; i < v; i++)
		{
			drawDepthLine(Points[i].x, Points[i].y, Points[i].w, Points[i + 1].x, Points[i + 1].y, Points[i + 1].w,  color); // used to make hollow outlines of a polygon
		}
		for (i = 0; i < c; i += 2)
		{
			drawDepthLine(inter[i], s, interW[i], inter[i + 1], s, interW[i], color);  // Used to fill the polygon ....
		}
	}
	
}

void fillAAPolygon(vector<vec3d> Points, uint32_t color);

void window::DrawRect(int x, int y, int size_x, int size_y, uint32_t color) {
	int i, j;
	for (j = 0; j < size_y; j++) {
		for (i = 0; i < size_x; i++) {
			putPixel((x + i), (y + j), color);
		}
	}
}

void window::drawPolygon(vector<vec3d> Points, uint32_t color) {
	size_t size = Points.size();
	Points.push_back(Points[0]);
	// Now do the actual drawing.
	for (int i = 0; i < size; i++) {
		vec3d point = Points[i];
		vec3d npoint = Points[i + 1];
		/*if (TestClip((int)point.x, (int)point.y) || TestClip(npoint.x, npoint.y))*/	drawLine(point.x, point.y, npoint.x, npoint.y, color);
	}
}

void window::drawDepthPolygon(vector<vec3d> Points, uint32_t color) {
	size_t size = Points.size();
	Points.push_back(Points[0]);
	// Now do the actual drawing.
	for (int i = 0; i < size; i++) {
		vec3d point = Points[i];
		vec3d npoint = Points[i + 1];
		/*if (TestClip((int)point.x, (int)point.y) || TestClip(npoint.x, npoint.y))*/	drawDepthLine(point.x, point.y, point.w, npoint.x, npoint.y, npoint.w, color);
	}
}
void window::drawAAPolygon(vector<vec3d> Points, uint32_t color) {
	size_t size = Points.size();
	Points.push_back(Points[0]);
	// Now do the actual drawing.
	for (int i = 0; i < size; i++) {
		vec3d point = Points[i];
		vec3d npoint = Points[i + 1];
		drawAALine(point.x, point.y, npoint.x, npoint.y, color);
	}
}

void window::setIcon(string path) {
	Icon = SDL_LoadBMP(path.data());
	SDL_SetWindowIcon(Window, Icon);
}

bool window::PollEvent() {
	int r = SDL_PollEvent(&e);
	if (e.type == MOUSEPRESSED) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			focused = nullptr;
			int mousex, mousey;
			SDL_GetMouseState(&mousex, &mousey);
			for (int i = 0; i < widgets.size(); i++) {
				if (widgets[i]->Type & WIDGET_TYPE_CLICKABLE) {
					if (mousex >= widgets[i]->x &&
						mousey >= widgets[i]->y &&
						mousex <= widgets[i]->x + widgets[i]->sizex &&
						mousey <= widgets[i]->y + widgets[i]->sizey) {
						((Button*)widgets[i])->OnClick(((Button*)widgets[i])->ptr);
						break;
					}
				}
				else if (widgets[i]->Type & WIDGET_TYPE_FOCUSABLE) {
					if (mousex >= widgets[i]->x &&
						mousey >= widgets[i]->y &&
						mousex <= widgets[i]->x + widgets[i]->sizex &&
						mousey <= widgets[i]->y + widgets[i]->sizey) {
						focused = widgets[i];
						break;
					}
				}
				else if (widgets[i]->Type & WIDGET_TYPE_DRAGGABLE) {
					if (mousex >= widgets[i]->x &&
						mousey >= widgets[i]->y &&
						mousex <= widgets[i]->x + widgets[i]->sizex &&
						mousey <= widgets[i]->y + widgets[i]->sizey) {
						focused = widgets[i];
						break;
					}
				}
			}
			isMouseDown = true;
		}
	}
	else if (e.type == MOUSERELEASED) {
		isMouseDown = false;
	}
	else if (e.type == MOUSEMOTION) {
		if (focused != nullptr) {
			if (isMouseDown) {
				if (focused->Type & WIDGET_TYPE_DRAGGABLE) {
					((Scale*)focused)->Drag(focused->ptrToWidget, e.motion.x, e.motion.y);
					((Scale*)focused)->OnDrag(focused->ptrToWidget, e.motion.x, e.motion.y);
				}
			}
		}
	}
	else if (e.type == KEYDOWN) {
		if (focused != nullptr) {
			if (focused->Type & WIDGET_TYPE_KBTYPABLE) {
				if (e.key.keysym.scancode == 42) {
					if (((Entry*)focused)->value.size() > 0)
					((Entry*)focused)->value.pop_back();
				}
				else ((Entry*)focused)->value += e.key.keysym.sym;
			}
		}
	}
	else if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
		
		int posx = 0, posy = 0;
		SDL_GetWindowPosition(Window, &posx, &posy);
		
		SDL_DestroyWindow(Window);
		SDL_Quit();

		SCREEN_WIDTH = e.window.data1;
		SCREEN_HEIGHT = e.window.data2;

		Window = nullptr;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			return r;
		}
		else {
			Window = SDL_CreateWindow(w_name.data(), posx, posy, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
			screenSurface = SDL_GetWindowSurface(Window);
			if (Window == nullptr) {
				return r;
			}
			else {
				SDL_UpdateWindowSurface(Window);
				bool quit = false;
				created = 1;
			}
		}
	}
	return r;
}

void window::setMousePos(int x, int y) {
	SDL_WarpMouseInWindow(Window, x, y);
}

void window::ShowCursor() {
	SDL_ShowCursor(SDL_ENABLE);
}

void window::HideCursor() {
	SDL_ShowCursor(SDL_DISABLE);
}

void window::setFullScreen() {
	SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN);
}

void window::setFullScreenDesktop() {
	SDL_SetWindowFullscreen(Window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void window::setWindowed() {
	SDL_SetWindowFullscreen(Window, 0);
}

void window::DrawChar(char c, uint16_t x, uint16_t y, uint32_t color, uint8_t size) {
	uint8_t i, j;
	// Draw pixels
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 8; i++) {
			if (reverse_byte(__font_bitmap__[c * 8 + j]) & (1 << i)) {
				DrawRect(x + (i * size), y + (j * size), size, size, color);
			}
		}
	}
}

void window::DrawString(string _string, uint16_t x, uint16_t y, uint32_t color, uint8_t size) {
	const char* str = _string.c_str();
	while (*str) {
		DrawChar(*str++, x, y, color, size);
		x += (8 * size);
	}
}

void window::DrawButtons() {
	/* for (Button & b : buttons) {
		DrawRect(b.x - 2, b.y - 2, b.sizex + 2, b.sizey + 2, 0xAAAAAA);
		DrawRect(b.x, b.y, b.sizex, b.sizey, b.color);
		DrawString(b.text, b.x, b.y, 0, 1);
	}*/
}

void window::DrawWidgets() {
	if (widgets.size() == 0) return;
	for (auto& b : widgets) {
		if (b->visible) {
			int x = 0, y = 0;
			switch (b->anchor) {
				case ANCHOR::NORTH_WEST:
					x = b->x;
					y = b->y;
					break;
				case ANCHOR::NORTH_EAST:
					x = b->x - (SCREEN_BASE_WIDTH - SCREEN_WIDTH);
					y = b->y;
					break;
				case ANCHOR::SOUTH_WEST:
					x = b->x;
					y = b->y - (SCREEN_BASE_HEIGHT - SCREEN_HEIGHT);
					break;
				case ANCHOR::SOUTH_EAST:
					x = b->x - (SCREEN_BASE_WIDTH - SCREEN_WIDTH);
					y = b->y - (SCREEN_BASE_HEIGHT - SCREEN_HEIGHT);
					break;
			}
			b->draw(this, b->ptrToWidget, x, y);
		}
	}
}

void window::updateWidget(size_t id, BaseWidget button) {
	//buttons[id] = button;
}

void window::RenderImage(Image i) {
	SDL_Rect base = { 0, 0, i.sizex, i.sizey };
	SDL_Rect rect = {i.x, i.y, i.sizex, i.sizey};
	SDL_BlitSurface(i.image, &base, screenSurface, &rect);
}

void window::ClearDepthBuffer() {
	depthBuffer.clear();
	for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
		depthBuffer.push_back(0.0f);
	}
}

SDL_Window** window::getSDLWindow() {
	return &Window;
}

bool window::isPersistant() {
	return persist;
}
void window::setPersistance(bool persistant) {
	persist = persistant;
}

SDL_Surface* window::getsScreenSurface() {
	return screenSurface;
}