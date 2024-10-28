#include <Arduino.h>
#include <Wire.h>
#include "hw.h"
#include "calc.h"
#include "ui.h"

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

std::array<Func,5> func_menu_list = {
	alt_text(KEY_1, 0, "sin("),
	alt_text(KEY_2, 0, "cos("),
	alt_text(KEY_3, 0, "tg("),
	alt_text(KEY_4, 0, "pi"),
	alt_text(KEY_5, 0, "e"),
};

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
			open_menu(&func_menu_list.front(), func_menu_list.size());
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
