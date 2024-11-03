#pragma once
#include <WString.h>

template <typename T> T read_config_value(const char* path, T default_value);
template <typename T> void write_config_value(const char* path, T value);
String read_config_string(const char* path, const String default_value);
void write_config_string(const char* path, String value);