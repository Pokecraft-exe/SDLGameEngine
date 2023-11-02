#pragma once
#include <stdint.h>

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
