#pragma once
#include <U8g2lib.h>
#include "keypad.h"

// Screen
#define SCREEN_W 128
#define SCREEN_H 64

struct FontInfo {
	const uint8_t *font;
	uint8_t w;
	uint8_t h;
};

const FontInfo font_default = {u8g2_font_spleen8x16_mf, 8, 16};
const FontInfo font_small = {u8g2_font_spleen5x8_mf, 5, 8};

void set_font(FontInfo font);

bool init_screen();

// Keypad
#define KEYPAD_ROWS 4
#define KEYPAD_COLUMNS 4
#define KEYPAD_SIZE (KEYPAD_ROWS*KEYPAD_COLUMNS)
const char charmap[KEYPAD_SIZE] = {'1','2','3','A','4','5','6','B','7','8','9','C','*','0','#','D'};
enum KeypadKey : uint16_t {
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_A,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_B,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_C,
	KEY_STAR,
	KEY_0,
	KEY_HASH,
	KEY_D,

	KEY_PREV = KEY_4,
	KEY_NEXT = KEY_6,
	KEY_SELECT = KEY_5,

	KEY_CANCEL = KEY_0,
	KEY_CONFIRM = KEY_STAR,

	KEY_LEFT = KEY_4,
	KEY_RIGHT = KEY_6,
	KEY_UP = KEY_2,
	KEY_DOWN = KEY_8,
};
void init_keypad();


extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern Keypad keypad;
extern FontInfo cfont;