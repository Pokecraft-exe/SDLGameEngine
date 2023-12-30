#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stddef.h>
#include <string>
#include "Color.h"

typedef void DRAW_CALL(void*, void*, int, int);

#define WIDGET_TYPE_NULL 0b00000000
#define WIDGET_TYPE_CLICKABLE 0b00000001
#define WIDGET_TYPE_KBTYPABLE 0b00000010
#define WIDGET_TYPE_FOCUSABLE 0b00000100
#define WIDGET_TYPE_DRAGGABLE 0b00001000

enum ANCHOR {
	NORTH_WEST = 0,
	NORTH_EAST,
	SOUTH_WEST,
	SOUTH_EAST,
};

class BaseWidget {
public:
	BaseWidget() {};
	BaseWidget(const BaseWidget& b);
	BaseWidget(DRAW_CALL* _draw, int _x, int _y, int _sizex, int _sizey, ANCHOR _anchor, size_t _id, void* ptr);
	void attach(void* w);
	void* ptrToWidget;
	DRAW_CALL* draw;
	int x = 0;
	int y = 0;
	int sizex = 0;
	int sizey = 0;
	int Type = WIDGET_TYPE_NULL;
	ANCHOR anchor = NORTH_WEST;
	size_t id = 0;
	bool visible = true;
};



class Label : public BaseWidget {
public:
	Label() {}
	Label(string text, Color color,  int x, int y);
	string text;
	Color color;
};

class Image : public BaseWidget {
public:
	Image() {}
	Image(string image, int x, int y);
	SDL_Surface* image;
};



typedef int FUNCTION_CALL(void*);

class Button : public BaseWidget {
public:
	Button() {}
	Button(string text, int x, int y, int sizex, int sizey, Color color, FUNCTION_CALL* OnClick, void* ptr);
	string text;
	Color color;
	FUNCTION_CALL* OnClick;
	void* ptr;
};



class Entry : public BaseWidget {
public:
	Entry() {}
	Entry(string placeHolder, int x, int y, int sizex, int sizey); 
	string value, placeHolder;
};



class CheckBox : public Button {
public:
	CheckBox() {}
	CheckBox(string text, int x, int y);
	bool checked = false;
};



typedef int DRAG_CALL(void*, int, int);

void defaultOnDrag(void*, int, int);

class Scale : public BaseWidget {
public:
	Scale() {};
	Scale(int x, int y, int sizex, int sizey, int max, bool horizontal = true, DRAG_CALL* onDrag = nullptr);
	bool horizontal;
	DRAG_CALL* Drag;
	DRAG_CALL* OnDrag = nullptr;
	void* ptr;
	float value = 0;
	int max = 100;
};

class EndWidget : public BaseWidget {
public:
	EndWidget();
};