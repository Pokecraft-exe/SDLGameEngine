#pragma once
#include <functional>
#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <stddef.h>
#include <string>
#include <type_traits>
#include <vector>
#include "Color.h"
#include "font.h"
#include "maths_engine.h"
#include "widgets.h"
using namespace std;
#define QUIT SDL_QUIT
#define KEYDOWN SDL_KEYDOWN
#define KEYUP SDL_KEYUP
#define MOUSEMOTION SDL_MOUSEMOTION
#define MOUSEPRESSED SDL_MOUSEBUTTONDOWN
#define MOUSERELEASED SDL_MOUSEBUTTONUP

#define GUI_BUTTONPRESSED 0x9345

class window {
private:
	SDL_Window* Window;
	SDL_Surface* screenSurface;
	SDL_Surface* Icon;
	string w_name;
	bool persist;
	unsigned int SCREEN_HEIGHT = 480;
	unsigned int SCREEN_WIDTH = 640;
	unsigned int SCREEN_BASE_HEIGHT = 480;
	unsigned int SCREEN_BASE_WIDTH = 640;
	bool isMouseDown = false;
public:
	BaseWidget* focused;
	vector<BaseWidget*> widgets;
	vector<float> depthBuffer;
	uint8_t pixelSize = 1;
	bool created = 0;
	SDL_Event e;
	window();
	window(const char* name);
	window(const char* name, unsigned int w, unsigned int h);
	int sizex() { return SCREEN_WIDTH; }
	int sizey() { return SCREEN_HEIGHT; }
	int baseSizex() { return SCREEN_BASE_WIDTH; }
	int baseSizey() { return SCREEN_BASE_HEIGHT; }
	void putPixel(unsigned int x, unsigned int y, uint32_t color);
	Color getPixel(unsigned int x, unsigned int y);
	void update();
	void resizable();
	bool TestClip(int x, int y);
	void Clip(int& x, int& y);
	void changeSize(unsigned int w, unsigned int h);
	void changeName(string name);
	void DrawRect(int x, int y, int size_x, int size_y, uint32_t color);
	void clear();
	void fill(Color c);
	void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
	void drawDepthLine(int x1, int y1, float w1, int x2, int y2, float w2, uint32_t c);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void drawAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void drawPolygon(vector<vec3d> Points, uint32_t color);
	void drawDepthPolygon(vector<vec3d> Points, uint32_t color);
	void drawAAPolygon(vector<vec3d> Points, uint32_t color);
	void quit();
	void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void fillAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void fillPolygon(vector<vec3d> Points, uint32_t color);
	void fillDepthPolygon(vector<vec3d> Points, uint32_t color);
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
	void DrawChar(char c, uint16_t x, uint16_t y, uint32_t color, uint8_t size);
	void DrawString(string _string, uint16_t x, uint16_t y, uint32_t color, uint8_t size);
	void DrawButtons();
	void DrawWidgets();
	void updateWidget(size_t id, BaseWidget widget);
	void RenderImage(Image i);
	void ClearDepthBuffer();
	SDL_Window** getSDLWindow();
	bool isPersistant();
	void setPersistance(bool persistant);
	SDL_Surface* getsScreenSurface();
};

