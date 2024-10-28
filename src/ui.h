// #pragma once
#include "hw.h"

extern uint8_t cmod;

struct Func {
	KeypadKey key;
	uint8_t modifiers;
	const char* name;
	std::function<void()> trigger;
};

void open_menu(Func* func_list, size_t n);
void draw_expr();
void draw_mod();