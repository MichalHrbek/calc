#include "ui.h"
#include "calc.h"

void open_menu(const Func* func_list, size_t n) {
	u8g2.setFont(font_small.font);
	u8g2.clearBuffer();
	u8g2.setCursor(0,0);
	for (size_t i = 0; i < n; i++)
	{
		Func f = func_list[i];
		if (i) u8g2.print(" ");
		if (u8g2.getStrWidth(f.name) + u8g2.getCursorX() > SCREEN_W) u8g2.setCursor(0,u8g2.getCursorY()+font_small.h);
		u8g2.printf("%c-%s", charmap[f.key], f.name);
	}
	u8g2.sendBuffer();
	keypad.wait_until_released();
	uint16_t func_key = keypad.wait_for_key();
	for (size_t i = 0; i < n; i++)
	{
		Func f = func_list[i];
		if (f.key == func_key) f.trigger();
	}
	u8g2.setFont(font_default.font);
	draw_expr();
	keypad.wait_until_released(func_key);
}

void draw_mod(uint8_t mod) {
	for (size_t i = 0; i < sizeof(uint8_t)*8; i++)
	{
		if (mod & (1<<i)) {
			u8g2.drawButtonUTF8(i*10+1,0, U8G2_BTN_INV, 0,  1, 1, mod_names[i]);
		}
	}
}

void print_mod(uint8_t mod) {
	if (!mod) return;
	u8g2.print('[');
	for (size_t i = 0; i < sizeof(uint8_t)*8; i++)
	{
		if (mod & (1<<i)) {
			u8g2.print(mod_names[i]);
		}
	}
	u8g2.print(']');
}

void draw_expr() {
	u8g2.clearBuffer();
	draw_mod(cmod);
	curr_expr.draw();
	u8g2.sendBuffer();
}