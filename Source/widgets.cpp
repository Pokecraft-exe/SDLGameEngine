#include "window.h"

BaseWidget::BaseWidget(const BaseWidget& b) {
	*this = b;
}

BaseWidget::BaseWidget(DRAW_CALL* _draw, int _x, int _y, int _sizex, int _sizey, ANCHOR _anchor, size_t _id, void* ptr) {
	draw = _draw;
	x = _x;
	y = _y;
	sizex = _sizex;
	sizey = _sizey;
	anchor = _anchor;
	id = _id;
	ptrToWidget = ptr;
}

void BaseWidget::attach(void* w) {
	id = ((window*)w)->widgets.size();
	((window*)w)->widgets.push_back(this);
}



void drawLabel(void* w, void* _this, int x, int y) {
	((window*)w)->DrawString(((Label*)_this)->text, x, y, ((Label*)_this)->color, 2);
}

Label::Label(string _text, Color c, int _x, int _y) {
	text = _text;
	color = c;
	x = _x;
	y = _y;
	sizex = 11 * text.size();
	sizey = 11;
	draw = drawLabel;
	ptrToWidget = this;
};



void drawImage(void* w, void* _this, int x, int y) {
	SDL_Rect base = { 0, 0, ((Image*)_this)->sizex, ((Image*)_this)->sizey };
	SDL_Rect rect = { x, y, ((Image*)_this)->sizex, ((Image*)_this)->sizey };
	SDL_BlitSurface(((Image*)_this)->image, &base, ((window*)w)->getsScreenSurface(), &rect);
}

Image::Image(string _image, int _x, int _y) {
	image = IMG_Load(_image.data());
	x = _x;
	y = _y;
	sizex = image->w;
	sizey = image->h;
	draw = drawImage;
	ptrToWidget = this;
};



void drawButton(void* w, void* _this, int x, int y) {
	((window*)w)->DrawRect(x - 2, y - 2, ((Button*)_this)->sizex + 2, ((Button*)_this)->sizey + 2, 0xAAAAAA);
	((window*)w)->DrawRect(x, y, ((Button*)_this)->sizex, ((Button*)_this)->sizey, ((Button*)_this)->color);
	((window*)w)->DrawString(((Button*)_this)->text, x, y, 0, 1);
}

Button::Button(string _text, int _x, int _y, int _sizex, int _sizey, Color _color, FUNCTION_CALL* _OnClick, void* _ptr) {
	text = _text;
	x = _x;
	y = _y;
	sizex = _sizex;
	sizey = _sizey;
	color = _color;
	OnClick = _OnClick;
	ptr = _ptr;
	Type = WIDGET_TYPE_CLICKABLE;
	draw = drawButton;
	ptrToWidget = this;
}


void drawEntry(void* w, void* _this, int x, int y) {
	((window*)w)->DrawRect(x - 2, y - 2, ((Entry*)_this)->sizex + 2, ((Entry*)_this)->sizey + 2, 0xAAAAAA);
	((window*)w)->DrawRect(x, y, ((Entry*)_this)->sizex, ((Entry*)_this)->sizey, 0xffffff);
	if (((Entry*)_this)->value == "") {
		((window*)w)->DrawString(((Entry*)_this)->placeHolder, x, y, 0x555555, 1);
	}
	else {

		((window*)w)->DrawString(((Entry*)_this)->value, x, y, 0, 1);
	}
}

Entry::Entry(string _placeHolder, int _x, int _y, int _sizex, int _sizey) {
	placeHolder = _placeHolder;
	value = "";
	x = _x;
	y = _y;
	sizex = _sizex;
	sizey = _sizey;
	draw = drawEntry;
	ptrToWidget = this;
	Type = WIDGET_TYPE_FOCUSABLE | WIDGET_TYPE_KBTYPABLE;
}



void drawCheckBox(void* w, void* _this, int x, int y) {
	((window*)w)->DrawRect(x - 2, y - 2, ((CheckBox*)_this)->sizex + 2, ((CheckBox*)_this)->sizey + 2, 0xAAAAAA);
	((window*)w)->DrawRect(x, y, ((CheckBox*)_this)->sizex, ((CheckBox*)_this)->sizey, 0xffffff);
	if (((CheckBox*)_this)->checked) {
		((window*)w)->DrawChar('X', x + 1, y + 1, 0, 1);
	}
	((window*)w)->DrawString(((CheckBox*)_this)->text, x + 15, y, 0, 1);
}

void CheckBoxOnClick(void* _this) {
	((CheckBox*)_this)->checked = !((CheckBox*)_this)->checked;
}

CheckBox::CheckBox(string _text, int _x, int _y) {
	text = _text;
	x = _x;
	y = _y;
	sizex = 10;
	sizey = 10;
	color = 0;
	OnClick = (FUNCTION_CALL*)CheckBoxOnClick;
	ptr = this;
	Type = WIDGET_TYPE_CLICKABLE;
	draw = drawCheckBox;
	ptrToWidget = this;
}



void DragScale(void* _this, int Horizontal, int Vertical) {
	if (((Scale*)_this)->horizontal) {
		float offset = Horizontal - ((Scale*)_this)->x;
		((Scale*)_this)->value = offset * (float)((Scale*)_this)->max / (float)((Scale*)_this)->sizex;
	}
	else {
		float offset = Vertical - ((Scale*)_this)->y;
		((Scale*)_this)->value = offset * (float)((Scale*)_this)->max / (float)((Scale*)_this)->sizey;
	}

	if (((Scale*)_this)->value < 0) ((Scale*)_this)->value = 0;
	if (((Scale*)_this)->value > ((Scale*)_this)->max) ((Scale*)_this)->value = ((Scale*)_this)->max;
}

void defaultOnDrag(void* a, int b, int c) { return; }

void DrawScale(void* w, void* _this, int x, int y) {
	((window*)w)->DrawRect(x - 2, y - 2, ((Scale*)_this)->sizex + 2, ((Scale*)_this)->sizey + 2, 0xAAAAAA);
	((window*)w)->DrawRect(x, y, ((Scale*)_this)->sizex, ((Scale*)_this)->sizey, 0xffffff);

	if (((Scale*)_this)->horizontal) {
		int offset = (((Scale*)_this)->value * (((Scale*)_this)->sizex - 20)) / ((Scale*)_this)->max;
		((window*)w)->DrawRect(x + offset - 2, y - 2, 22, ((Scale*)_this)->sizey + 2, 0xAAAAAA);
		((window*)w)->DrawRect(x + offset, y, 20, ((Scale*)_this)->sizey, 0x666666);
	}
	else {
		int offset = (((Scale*)_this)->value * (((Scale*)_this)->sizey - 20)) / ((Scale*)_this)->max;
		((window*)w)->DrawRect(x - 2, y + offset - 2, ((Scale*)_this)->sizex + 2, 22, 0xAAAAAA);
		((window*)w)->DrawRect(x, y + offset, ((Scale*)_this)->sizex, 20, 0x666666);
	}
}

Scale::Scale(int _x, int _y, int _sizex, int _sizey, int _max, bool _horizontal, DRAG_CALL* _onDrag) {
	draw = (DRAW_CALL*)DrawScale;
	x = _x;
	y = _y;
	sizex = _sizex;
	sizey = _sizey;
	ptrToWidget = this;
	horizontal = _horizontal;
	Drag = (DRAG_CALL*)DragScale;
	OnDrag = OnDrag;
	if (OnDrag == nullptr) OnDrag = (DRAG_CALL*)defaultOnDrag;
	max = _max;
	Type = WIDGET_TYPE_DRAGGABLE;
}



void drawEndWidget(void* a, void* b, int c, int d) { return; }

EndWidget::EndWidget() {
	draw = (DRAW_CALL*)drawEndWidget;
	ptrToWidget = this;
}