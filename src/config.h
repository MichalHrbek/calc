#pragma once
#include <WString.h>
#include <SPIFFS.h>

// Make sure to keep the byte order correct

template <typename T>
T read_config_value(const char* path, T default_value)
{
	File file = SPIFFS.open(path);
	if (file)
	{
		if (file.size() < sizeof(T)) return default_value;
		T value;
		file.readBytes((char*)&value,sizeof(T));
		return value;
	}
	return default_value;
}

template <typename T>
void write_config_value(const char* path, T value)
{
	File file = SPIFFS.open(path, FILE_WRITE, true);
	if (file) file.write((uint8_t*)&value,sizeof(T));
}

String read_config_string(const char* path, const String default_value="");
void write_config_string(const char* path, String value);
void config_menu();