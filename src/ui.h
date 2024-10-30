// #pragma once
#include "hw.h"
#include <FS.h>

struct Func {
	KeypadKey key;
	uint8_t modifiers;
	const char* name;
	std::function<void()> trigger;
};

void open_menu(const Func* func_list, size_t n);
void draw_expr();
void draw_mod();
File file_menu(const char* path);
void show_file(File f);