#include "ui.h"
#include "calc.h"
#include <SPIFFS.h>
#include <vector>
#include <Arduino.h>
#include "utils.h"

void open_menu(const Func* func_list, size_t n) {
	u8g2.clearBuffer();
	u8g2.setCursor(0,0);
	for (size_t i = 0; i < n; i++)
	{
		Func f = func_list[i];
		if (i) u8g2.print(" ");
		if ((u8g2.getStrWidth(f.name)+2) + u8g2.getCursorX() > SCREEN_W) line_break();
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
	if (!mod) return;
	u8g2.setCursor(0,0);
	u8g2.setColorIndex(0);
	for (size_t i = 0; i < sizeof(uint8_t)*8; i++)
	{
		if (mod & (1<<i)) {
			u8g2.print(' ');
			u8g2.print(mod_names[i]);
			u8g2.print(' ');
		}
	}
	u8g2.setColorIndex(1);
}

void draw_expr() {
	u8g2.clearBuffer();
	draw_mod(cmod);
	curr_expr.draw();
	u8g2.sendBuffer();
}

int32_t list_menu(const char *options[], size_t n) {
	int32_t selected = 0;
	while (true) {
		keypad.wait_until_released();
		selected = _max(_min(selected,(int32_t)n-1),0);
		u8g2.clearBuffer();
		u8g2.setCursor(0, 0);

		for (size_t i = selected; i < n; i++)
		{
			u8g2.print(options[i]);
			if (i == selected) u8g2.print(" <-");
			line_break();
			if (u8g2.getCursorY() > SCREEN_H) break;
		}
		u8g2.sendBuffer();
		
		// Input
		uint16_t key = keypad.wait_for_release();
		if (key == KEY_PREV || key == KEY_UP) selected--; // Prev
		else if (key == KEY_NEXT || key == KEY_DOWN) selected++; // Next
		else if (key == KEY_SELECT || key == KEY_CONFIRM) return selected; // Select
		else if (key == KEY_CANCEL) return -1; // Cancel
	}
}

File file_menu(const char* path, bool show_path_prefix) {
	File root = SPIFFS.open(path);
	std::vector<File> files; // Making sure the names don't get freed
	std::vector<const char*> names;
	size_t prefix_len = strlen(path);
	while (File f = root.openNextFile()) {
		files.push_back(f);
		names.push_back(show_path_prefix ? f.path() : (f.path()+prefix_len));
	}
	int32_t selected = list_menu(names.data(), names.size());
	
	if (selected == -1) return File(nullptr);
	return files[selected];
}

void show_file(File f) {
	Serial.printf("Reading: %s\n", f.path());
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
					line_break();
					if (u8g2.getCursorY() > SCREEN_H) break;
				}
			}
			if (line_pos >= 0) u8g2.print(c);
		}
		if (line_pos < 0) {
			scroll--;
			continue;
		}
		
		u8g2.sendBuffer();

		// Input
		uint16_t key = keypad.wait_for_release();
		if (key == KEY_UP) scroll =  _max(scroll-1,0);
		else if (key == KEY_DOWN) scroll++;
		else if (key == KEY_CANCEL || key == KEY_CONFIRM) break; // Cancel
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

		if (u8g2.getCursorX()+cfont.w >= SCREEN_W) line_break();
	}
}

std::string text_input(const char* default_value, const char* prompt)
{
	keypad.wait_until_released();
	ScopedFontChange c(font_small);
	std::string text(default_value);
	int16_t selected = 0;
	uint8_t line_w = SCREEN_W/cfont.w;
	bool capital = false;
	
	while (true) {
		// Drawing
		u8g2.clearBuffer();
		u8g2.setCursor(0,0);
		if (prompt)
		{
			ScopedColorChange c(0);
			u8g2.print(prompt);
			u8g2.print(":");
		}
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
		case KEY_CONFIRM:
			return text;
			break;
		default:
			break;
		}
		if (selected > 68 || selected < 0) selected = prev;
	}
}

void announce(const char *msg)
{
	u8g2.clearBuffer();
	u8g2.setCursor(0,0);
	size_t len = strlen(msg);
	size_t pos = 0;
	while (pos < len)
	{
		u8g2.print(msg[pos++]);
		if (msg[pos] == '\n' || u8g2.getCursorX() + cfont.w > SCREEN_W) line_break();
	}
	u8g2.sendBuffer();
	keypad.wait_for_release();	
}