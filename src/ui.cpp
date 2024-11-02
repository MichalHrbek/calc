#include "ui.h"
#include "calc.h"
#include <SPIFFS.h>
#include <vector>
#include <Arduino.h>

void open_menu(const Func* func_list, size_t n) {
	u8g2.clearBuffer();
	u8g2.setCursor(0,0);
	for (size_t i = 0; i < n; i++)
	{
		Func f = func_list[i];
		if (i) u8g2.print(" ");
		if (u8g2.getStrWidth(f.name) + u8g2.getCursorX() > SCREEN_W) u8g2.setCursor(0,u8g2.getCursorY()+cfont.h);
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
}

void draw_mod(uint8_t mod) {
	for (size_t i = 0; i < sizeof(uint8_t)*8; i++)
	{
		if (mod & (1<<i)) {
			u8g2.drawButtonUTF8(i*(cfont.w+2)+1,0, U8G2_BTN_INV, 0,  1, 1, mod_names[i]);
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

int32_t list_menu(const char* const* options, size_t n) {
	int32_t selected = 0;
	while (true) {
		keypad.wait_until_released();
		selected = _max(_min(selected,(int32_t)n-1),0);
		u8g2.clearDisplay();
		u8g2.setCursor(0, 0);

		for (size_t i = selected; i < n; i++)
		{
			u8g2.print(options[i]);
			if (i == selected) u8g2.print(" <-");
			u8g2.setCursor(0,u8g2.getCursorY()+cfont.h);
			if (u8g2.getCursorY() > SCREEN_H) break;
		}
		u8g2.sendBuffer();
		
		// Input
		uint16_t key = keypad.wait_for_key();
		if (key == KEY_PREV) selected--; // Prev
		else if (key == KEY_NEXT) selected++; // Next
		else if (key == KEY_SELECT) return selected; // Select
		else if (key == KEY_CANCEL) return -1; // Cancel
	}
}

File file_menu(const char* path) {
	File root = SPIFFS.open(path);
	std::vector<File> files; // Making sure the names don't get freed
	std::vector<const char*> names;
	while (File f = root.openNextFile()) {
		files.push_back(f);
		names.push_back(f.name());
	}
	int32_t selected = list_menu(names.data(), names.size());
	
	if (selected == -1) return File(nullptr);
	return files[selected];
}

void show_file(File f) {
	Serial.printf("Reading: %s\n", f.name());
	int32_t scroll = 0;
	int32_t s = f.size();
	String text = f.readString();
	int32_t line_w = SCREEN_W/cfont.w;
	while (true)
	{
		u8g2.setCursor(0,0);
		u8g2.clearBuffer();
		int32_t line_pos = -scroll;
		int32_t ll = 0;
		for (size_t i = 0; i < s; i++)
		{
			char c = text.charAt(i);
			if (c == '\n' || (i-ll+1)*cfont.w > SCREEN_W)
			{
				ll = i;
				if (line_pos++ >= 0)
				{
					u8g2.setCursor(0,u8g2.getCursorY()+cfont.h);
					if (u8g2.getCursorY() > SCREEN_H) break;
				}
			}
			if (line_pos >= 0) u8g2.print(c);
		}
		
		u8g2.sendBuffer();

		// Input
		uint16_t key = keypad.wait_for_release();
		if (key == KEY_PREV) scroll =  _max(scroll-1,0); // Up
		else if (key == KEY_NEXT) scroll++; // Down
		else if (key == KEY_CANCEL) break; // Cancel
	}
	f.close();
}

char _pos_to_char(bool capital, uint8_t selected)
{
	selected += 32;
	if(selected > 96) selected += 26; 
	if(!capital && selected > 64 && selected < 91) selected+=32;
	return selected;
}

void draw_keyboard(bool capital, uint8_t selected)
{
	for (uint8_t i = 0; i < 127-32-26; i++)
	{
		if (selected == i)
		{
			u8g2.setDrawColor(0);
			u8g2.print(_pos_to_char(capital,i));
			u8g2.setDrawColor(1);
		}
		else u8g2.print(_pos_to_char(capital,i));

		if (u8g2.getCursorX()+cfont.w >= SCREEN_W) u8g2.setCursor(0,u8g2.getCursorY()+cfont.h);
	}
}

std::string text_input(const char* start)
{
	ScopedFontChange c(font_small);

	std::string text(start);
	int16_t selected = 0;
	uint8_t line_w = SCREEN_W/cfont.w;
	bool capital = false;
	
	while (true) {
		// Drawing
		u8g2.clearBuffer();
		u8g2.setCursor(0,0);
		u8g2.print(text.c_str());
		u8g2.setCursor(0,cfont.h*2);
		draw_keyboard(capital,selected);
		u8g2.sendBuffer();

		// Input
		int16_t prev = selected;
		uint16_t key = keypad.wait_for_release();
		switch (key)
		{
		case KEY_LEFT:
			if(selected % line_w) selected--;
			else if(selected==69-(69%line_w)) selected += 69%line_w-1;
			else selected += line_w-1;
			break;
		case KEY_RIGHT:
			if(!(++selected % line_w)) selected -= line_w;
			else if(selected==69) selected -= 69%line_w;
			break;
		case KEY_UP:
			selected-=line_w;
			break;
		case KEY_DOWN:
			selected+=line_w;
			break;
		case KEY_D:
			capital = !capital;
			break;
		case KEY_SELECT:
			text += _pos_to_char(capital,selected);
			break;
		case KEY_C:
			if(!text.empty()) text.pop_back();
			break;
		case KEY_0:
			return text;
			break;
		default:
			break;
		}
		if (selected > 68 || selected < 0) selected = prev;
	}
}