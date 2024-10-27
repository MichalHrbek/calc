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
	u8g2.setCursor(0,font_default.h);
	u8g2.print(text.c_str());
	// Eval
	if (error) {
		u8g2.setCursor((error-1)*font_default.w,2*font_default.h);
		u8g2.print('^');
	}
	u8g2.setCursor(0,3*font_default.h);
	u8g2.printf("%*f", u8g2.getCols(), result);
}

void Expression::clear()
{
	text = "";
	cursor = 0;
	error = 0;
	result = 0.0;
}

Expression curr_expr;