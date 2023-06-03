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
		drawLine(point.x, point.y, npoint.x, npoint.y, color);
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
			int mousex, mousey;
			SDL_GetMouseState(&mousex, &mousey);
			for (int i = 0; i < buttons.size(); i++) {
				if (mousex >= buttons[i].x && 
					mousey >= buttons[i].y &&
					mousex <= buttons[i].x + buttons[i].xend && 
					mousey <= buttons[i].y + buttons[i].yend) {
					buttons[i].OnClick(buttons[i].ptr);
				}
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

Button::Button(const Button& b) {
	text = b.text;
	x = b.x;
	y = b.y;
	xend = b.xend;
	yend = b.yend;
	color = b.color;
	OnClick = b.OnClick;
	ptr = b.ptr;
	id = b.id;
}

Button::Button(string _text, int _x, int _y, int _xend, int _yend, Color _color, BUTTON_CALL* _OnClick, void* _ptr, size_t _id) {
	text = _text;
	x = _x;
	y = _y;
	xend = _xend;
	yend = _yend;
	color = _color;
	OnClick = _OnClick;
	ptr = _ptr;
	id = _id;
}

Button* window::addButton(string text, int x, int y, int xend, int yend, Color color, BUTTON_CALL* OnClick, void* ptr) {
	buttons.push_back(Button(text, x, y, xend, yend, color, OnClick, ptr, buttons.size()));
	return &buttons[buttons.size()-1];
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
	const char* str = _string.data();
	while (*str) {
		DrawChar(*str++, x, y, color, size);
		x += (8 * size);
	}
}

void window::DrawButtons() {
	for (Button& b : buttons) {
		DrawRect(b.x - 2, b.y - 2, b.xend + 2, b.yend + 2, 0xAAAAAA);
		DrawRect(b.x, b.y, b.xend, b.yend, b.color);
		DrawString(b.text, b.x, b.y, 0, 1);
	}
}

void window::changeButton(size_t id, Button button) {
	buttons[id] = button;
}

Image::Image(const Image& i) {
	image = i.image;
	x = i.x;
	y = i.y;
	sizex = i.sizex;
	sizey = i.sizey;
	id = i.id;
}
Image::Image(string _image, int _x, int _y) {
	image = IMG_Load(_image.data());
	x = _x;
	y = _y;
	sizex = image->w;
	sizey = image->h;
};

void window::RenderImage(Image i) {
	SDL_Rect base = { 0, 0, i.sizex, i.sizey };
	SDL_Rect rect = {i.x, i.y, i.sizex, i.sizey};
	SDL_BlitSurface(i.image, &base, screenSurface, &rect);
}