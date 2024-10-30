#include "calc.h"
#include <Arduino.h>
#include "hw.h"

void Expression::eval() {
	result = te_interp(text.c_str(), &error);
}

void Expression::write(const std::string &input)
{
	text.insert(cursor,input);
	cursor += input.size();
}

void Expression::remove(uint16_t n)
{
	n = _min(cursor, n);
	text.erase(cursor-n,n);
	cursor -= n;
}

void Expression::draw()
{
	// Expr
	u8g2.setCursor(0,cfont.h);
	u8g2.print(text.c_str());
	// Error + Cursor
	if (error == cursor+1) {
		u8g2.setCursor((error-1)*cfont.w,2*cfont.h);
		u8g2.print((char)202);
	} else {
		// Error
		if (error) {
			u8g2.setCursor((error-1)*cfont.w,2*cfont.h);
			u8g2.print('E');
		}
		// Cursor
		u8g2.setCursor(cursor*cfont.w,2*cfont.h);
		u8g2.print('^');
	}
	// Eval
	u8g2.setCursor(0,3*cfont.h);
	u8g2.printf("%*f", u8g2.getCols(), result);
}

void Expression::clear()
{
	text = "";
	cursor = 0;
	error = 0;
	result = 0.0;
}

void Expression::move_cursor(int8_t delta)
{
	cursor += delta;
	// Wraps around only +1 and -1
	if (cursor < 0) cursor = text.size();
	if (cursor > text.size()) cursor = 0; // There might be some issues with comparing signed and unsigned (size_t)
}

Expression curr_expr;
uint8_t cmod = 0;