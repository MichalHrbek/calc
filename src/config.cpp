#include "config.h"
#include "ui.h"
#include "utils.h"

void write_config_string(const char* path, String value)
{
	File file = SPIFFS.open(path, FILE_WRITE, true);
	if (file) file.write((uint8_t*)value.c_str(),value.length());
}

String read_config_string(const char* path, const String default_value)
{
	File file = SPIFFS.open(path);
	if (file) return file.readString();
	return default_value;
}

void config_menu() {
	ScopedFontChange c(font_small);
	File f = file_menu("/config", false);
	if (!f) return;
	const char* o[] = {"Read", "Write"};
	switch (list_menu(o,arraySize(o)))
	{
	case 0:
		show_file(f);
		break;
	case 1:
	{
		String s = text_input(f.readString().c_str());
		f = SPIFFS.open(f.path(), FILE_WRITE);
		f.write((uint8_t*)s.c_str(),s.length());
		break;
	}
	default:
		return;
		break;
	}
}