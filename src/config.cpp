#include "config.h"
#include "ui.h"
#include "utils.h"
#include <SPIFFS.h>

template <typename T>
void write_config_value(const char* path, T value)
{
	File file = SPIFFS.open(path, FILE_WRITE);
	if (file) file.write(&value,sizeof(T));
}

void write_config_string(const char* path, String value)
{
	File file = SPIFFS.open(path, FILE_WRITE);
	if (file) file.write((uint8_t*)value.c_str(),value.length());
}

template <typename T>
T read_config_value(const char* path, T default_value)
{
	File file = SPIFFS.open(path);
	if (file)
	{
		if (file.size() < sizeof(T)) return default_value;
		T value;
		file.readBytes(&value,sizeof(T));
		return value;
	}
	else write_config_value<T>(path,default_value);
	return default_value;
}

String read_config_string(const char* path, const String default_value)
{
	File file = SPIFFS.open(path);
	if (file) return file.readString();
	else write_config_string(path,default_value);
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