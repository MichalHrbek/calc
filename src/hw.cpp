#include "hw.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);
FontInfo cfont = font_default;

void set_font(FontInfo font)
{
	cfont = font;
	u8g2.setFont(cfont.font);
}

bool init_screen()
{
	if (!u8g2.begin()) return false;
	set_font(cfont);
	u8g2.setFontRefHeightExtendedText();
	u8g2.setDrawColor(1);
	u8g2.setFontPosTop();
	u8g2.setFontDirection(0);
	return true;
}

const uint8_t rows[] = {1,2,4,6};
const uint8_t columns[] = {8,10,13,14};
Keypad keypad(KEYPAD_ROWS, KEYPAD_COLUMNS, rows, columns);

void init_keypad() {
	keypad.begin();
}