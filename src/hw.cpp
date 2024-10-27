#include "hw.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

bool init_screen() {
	if (!u8g2.begin()) return false;
	u8g2.setFont(font_default.font);
	u8g2.setFontRefHeightExtendedText();
	u8g2.setDrawColor(1);
	u8g2.setFontPosTop();
	u8g2.setFontDirection(0);
	return true;
}

uint8_t rows[] = {1,2,4,6};
uint8_t columns[] = {8,10,13,14};
Keypad keypad(KEYPAD_ROWS, KEYPAD_COLUMNS, rows, columns);

void init_keypad() {
	keypad.begin();
}