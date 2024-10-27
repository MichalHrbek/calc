#include <Arduino.h>
#include <Wire.h>
#include "hw.h"
#include "calc.h"

void setup() {
	Serial.begin(115200);
	Wire.begin(39,37);
	
	init_screen();
	init_keypad();

	u8g2.clearBuffer();
	u8g2.setCursor(0,0);
	u8g2.print("/");
	u8g2.sendBuffer();
}

uint8_t cmod = 0;

void draw_mod() {
	for (size_t i = 0; i < sizeof(uint8_t)*8; i++)
	{
		if (cmod & (1<<i)) {
			u8g2.drawButtonUTF8(i*10+1,0, U8G2_BTN_INV, 0,  1, 1, mod_names[i]);
		}
	}
}

void draw_expr() {
	u8g2.clearBuffer();
	draw_mod();
	curr_expr.draw();
	u8g2.sendBuffer();
}

struct Func {
	KeypadKey key;
	uint8_t modifiers;
	const char* name;
	std::function<void()> trigger;
};

inline Func alt_text(KeypadKey key, uint8_t modifiers, const char* text) {
	return {
		key,
		modifiers,
		text,
		[text,key](){
			curr_expr.write(text);
			keypad.wait_until_released(key);
		}
	};
}

std::array<Func,2> func_menu_list = {
	alt_text(KEY_1, 0, "sin("),
	alt_text(KEY_2, 0, "cos("),
};

void open_menu() {
	u8g2.setFont(font_small.font);
	u8g2.clearBuffer();
	u8g2.setCursor(0,0);
	for (size_t i = 0; i < func_menu_list.size(); i++)
	{
		Func f = func_menu_list.at(i);
		if (i) u8g2.print(" ");
		if (u8g2.getStrWidth(f.name) + u8g2.getCursorX() > SCREEN_W) u8g2.setCursor(0,u8g2.getCursorY()+font_small.h);
		u8g2.printf("%c-%s", charmap[f.key], f.name);
	}
	u8g2.sendBuffer();
	keypad.wait_until_released(KEY_D);
	uint16_t func_key = keypad.wait_for_key();
	for (size_t i = 0; i < func_menu_list.size(); i++)
	{
		Func f = func_menu_list.at(i);
		if (f.key == func_key) f.trigger();
	}
	u8g2.setFont(font_default.font);
	draw_expr();
	keypad.wait_until_released(func_key);
}

std::array<Func,11> base_key_list = {
	alt_text(KEY_A, 0, "+"),
	alt_text(KEY_B, 0, "-"),
	alt_text(KEY_HASH, 0, "/"),
	alt_text(KEY_STAR, MOD_SHIFT, "^"),
	alt_text(KEY_A, MOD_SHIFT, "("),
	alt_text(KEY_B, MOD_SHIFT, ")"),
	{
		KEY_HASH,
		MOD_SHIFT,
		"eval",
		[]() {
			curr_expr.eval();
		}
	},
	{
		KEY_C,
		0,
		"del",
		[]() {
			curr_expr.remove(1);
		}
	},
	{
		KEY_C,
		MOD_SHIFT,
		"clear",
		[]() {
			curr_expr.clear();
		}
	},
	{
		KEY_D,
		0,
		"shift",
		[]() {
			cmod ^= MOD_SHIFT;
		}
	},
	{
		KEY_D,
		MOD_SHIFT,
		"open menu",
		[]() {
			open_menu();
		}
	},
};

void loop(){
	uint16_t key = keypad.wait_for_key();
	bool found = false;
	for (size_t i = 0; i < base_key_list.size(); i++)
	{
		Func f = base_key_list[i];
		if (f.key == key && cmod == f.modifiers) {
			found = true;
			cmod = 0;
			Serial.printf("Action: %s\n", f.name);
			f.trigger();
			break;
		}
	}
	
	if(!found) {
		curr_expr.write(std::string(1,charmap[key]));
	}

	draw_expr();
	keypad.wait_until_released(key);
}
