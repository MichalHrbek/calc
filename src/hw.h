#include <U8g2lib.h>
#include "keypad.h"

extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;
extern Keypad keypad;

// Screen
#define FONT_W 8
#define FONT_H 16
#define SCREEN_W 128
#define SCREEN_H 64
#define TEXT_ROWS (SCREEN_H/FONT_H)
#define TEXT_COLUMNS (SCREEN_W/FONT_W)
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
};
void init_keypad();