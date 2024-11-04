#pragma once
#include "hw.h"
#include <FS.h>

struct Func {
	KeypadKey key;
	uint8_t modifiers;
	const char* name;
	std::function<void()> trigger;
};

class ScopedFontChange {
	private:
		FontInfo _prev_font;
	public:
		ScopedFontChange(FontInfo font)
		{
			_prev_font = cfont;
			set_font(font);
		}
		~ScopedFontChange()
		{
			set_font(_prev_font);
		}
};

class ScopedColorChange {
	private:
		uint8_t _prev_color;
	public:
		ScopedColorChange(uint8_t color)
		{
			_prev_color = u8g2.getColorIndex();
			u8g2.setColorIndex(color);
		}
		~ScopedColorChange()
		{
			u8g2.setColorIndex(_prev_color);
		}
};

void open_menu(const Func* func_list, size_t n);
void draw_expr();
void draw_mod();
int32_t list_menu(const char *options[], size_t n);
File file_menu(const char* path, bool show_path_prefix=true);
void show_file(File f);
std::string text_input(const char* default_value="", const char* prompt=nullptr);
void announce(const char* msg);
inline void line_break()
{
	u8g2.setCursor(0,u8g2.getCursorY()+cfont.h);
}