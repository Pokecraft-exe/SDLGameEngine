#pragma once
#include <functional>
#include <memory>
#include <SDL.h>
#include <stddef.h>
#include <string>
#include <type_traits>
#include <vector>
#include "Color.h"
#include "font.h"
#include "maths_engine.h"
using namespace std;
#define QUIT SDL_QUIT
#define KEYDOWN SDL_KEYDOWN
#define KEYUP SDL_KEYUP
#define MOUSEMOTION SDL_MOUSEMOTION
#define MOUSEPRESSED SDL_MOUSEBUTTONDOWN
#define MOUSERELEASED SDL_MOUSEBUTTONUP

#define GUI_BUTTONPRESSED 0x9345

typedef int BUTTON_CALL(void*);

struct Image {
	Image() {};
	Image(const Image& i);
	Image(string image, int x, int y);
	SDL_Surface* image;
	int x;
	int y;
	int sizex;
	int sizey;
	size_t id;
};

struct Button {
	Button() {};
	Button(const Button& b);
	Button(string text, int x, int y, int xend, int yend, Color color, BUTTON_CALL* OnClick, void* ptr, size_t id);
	string text;
	int x;
	int y;
	int xend = 9;
	int yend = 9;
	Color color;
	BUTTON_CALL* OnClick;
	void* ptr;
	size_t id;
};

class window {
private:
	SDL_Window* Window;
	SDL_Surface* screenSurface;
	SDL_Surface* Icon;
	string w_name;
public:
	vector<Button> buttons;
	unsigned int SCREEN_WIDTH = 640;
	uint8_t pixelSize = 1;
	unsigned int SCREEN_HEIGHT = 480;
	bool created = 0;
	SDL_Event e;
	window();
	window(const char* name);
	window(const char* name, unsigned int w, unsigned int h);
	void putPixel(unsigned int x, unsigned int y, uint32_t color);
	Color getPixel(unsigned int x, unsigned int y);
	void update();
	void Clip(unsigned int& x, unsigned int& y);
	void Clip(int& x, int& y);
	void changeSize(unsigned int w, unsigned int h);
	void changeName(string name);
	void DrawRect(int x, int y, int size_x, int size_y, uint32_t color);
	void clear();
	void fill(Color c);
	void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void drawAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void drawPolygon(vector<vec3d> Points, uint32_t color);
	void drawAAPolygon(vector<vec3d> Points, uint32_t color);
	void quit();
	void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void fillAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void fillPolygon(vector<vec3d> Points, uint32_t color);
	void fillAAPolygon(vector<vec3d> Points, uint32_t color);
	void setIcon(string path);
	void drawAALine(int x0, int y0, int x1, int y1, Color c);
	uint32_t Ablend(Color color1, Color color2);
	bool PollEvent();
	void setMousePos(int x, int y);
	void ShowCursor();
	void HideCursor();
	void setFullScreen();
	void setFullScreenDesktop();
	void setWindowed();
	Button* addButton(string text, int x, int y, int xend, int yend, Color color, BUTTON_CALL* OnClick, void* ptr);
	void DrawChar(char c, uint16_t x, uint16_t y, uint32_t color, uint8_t size);
	void DrawString(string _string, uint16_t x, uint16_t y, uint32_t color, uint8_t size);
	void DrawButtons();
	void changeButton(size_t id, Button button);
	void RenderImage(Image i);
};

