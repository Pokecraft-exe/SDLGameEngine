#pragma once
#include <SDL.h>
#include <stddef.h>
#include <string>
#include "maths_engine.h"

class Color {
private:
	uint8_t data[4];
public:
	Color(uint32_t color = 0x000000ff);
	Color(Color& color);
	uint32_t Ablend(Color fg, Color bg);
	uint8_t R();
	uint8_t G();
	uint8_t B();
	uint8_t A();
	void R(uint8_t n) { data[0]; };
	void G(uint8_t n) { data[1]; };
	void B(uint8_t n) { data[2]; };
	void A(uint8_t n) { data[3]; };
	operator uint32_t();
};

class window {
private:
	SDL_Window* Window;
	SDL_Surface* screenSurface;
	SDL_Surface* Icon;
	std::string w_name;
public:
	unsigned int SCREEN_WIDTH = 640;
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
	void changeName(std::string name);
	void clear();
	void drawLine(int x1, int y1, int x2, int y2, uint32_t color);
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void drawAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void quit();
	void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void fillAATriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
	void setIcon(std::string path);
	void drawAALine(int x0, int y0, int x1, int y1, Color c);
	uint32_t Ablend(Color color1, Color color2);
};

