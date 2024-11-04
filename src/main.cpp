#include <Arduino.h>
#include <Wire.h>
#include <SPIFFS.h>
#include "hw.h"
#include "calc.h"
#include "ui.h"
#include "utils.h"
#include "config.h"

void setup() {
	Serial.begin(115200);
	Wire.begin(39,37);

	init_screen();
	init_keypad();

	SPIFFS.begin(true);

	draw_expr();
}

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

const Func func_menu_list[] = {
	alt_text(KEY_1, 0, "sin("),
	alt_text(KEY_2, 0, "asin("),
	alt_text(KEY_3, 0, "cos("),
	alt_text(KEY_4, 0, "acos("),
	alt_text(KEY_5, 0, "tan("),
	alt_text(KEY_6, 0, "atan("),
	alt_text(KEY_7, 0, "abs("),
	alt_text(KEY_A, 0, "pi"),
	alt_text(KEY_B, 0, "e"),
	{
		KEY_STAR,
		0,
		"FILE",
		[]() {
			if (File f = file_menu("/")) {
				ScopedFontChange c(font_small);
				keypad.wait_until_released();
				show_file(f);
			}
		}
	},
	{
		KEY_HASH,
		0,
		"CONFIG",
		config_menu
	}
};

const Func base_key_list[] = {
	alt_text(KEY_A, 0, "+"),
	alt_text(KEY_B, 0, "-"),
	alt_text(KEY_HASH, 0, "/"),
	alt_text(KEY_STAR, MOD_SHIFT, "^"),
	alt_text(KEY_A, MOD_SHIFT, "("),
	alt_text(KEY_B, MOD_SHIFT, ")"),
	alt_text(KEY_9, MOD_SHIFT, "."),
	alt_text(KEY_1, MOD_SHIFT, "*10^"),
	alt_text(KEY_2, MOD_SHIFT, "sqrt("),
	{
		KEY_HASH,
		MOD_SHIFT,
		"eval",
		[]() {
			curr_expr.eval();
		}
	},
	{
		KEY_D,
		MOD_SHIFT,
		"unshift",
		[]() {}
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
			cmod |= MOD_SHIFT;
		}
	},
	{
		KEY_0,
		MOD_SHIFT,
		"open menu",
		[]() {
			set_font(font_small);
			open_menu(func_menu_list, arraySize(func_menu_list));
			set_font(font_default);
		}
	},
		{
		KEY_PREV,
		MOD_SHIFT,
		"move left",
		[]() {
			cmod |= MOD_SHIFT;
			curr_expr.move_cursor(-1);
		}
	},
	{
		KEY_NEXT,
		MOD_SHIFT,
		"move right",
		[]() {
			cmod |= MOD_SHIFT;
			curr_expr.move_cursor(1);
		}
	},
	{
		KEY_SELECT,
		MOD_SHIFT,
		"confirm",
		[]() {}
	},
};

void loop(){
	uint16_t key = keypad.wait_for_key();
	bool found = false;
	for (size_t i = 0; i < arraySize(base_key_list); i++)
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
	keypad.wait_until_released();
}
