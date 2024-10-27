#include "keypad.h"

#define REBOUND_TIME 100

void Keypad::poll()
{
	for (size_t i = 0; i < _n_columns; i++)
	{
		digitalWrite(_column_pins[i], LOW);
		for (size_t j = 0; j < _n_rows; j++)
		{
			if (digitalRead(_row_pins[j]) == LOW) {
				keys[i+j*_n_columns] = true;
			} else {
				keys[i+j*_n_columns] = false;
			}
		}
		digitalWrite(_column_pins[i], HIGH);
	}
}

uint16_t Keypad::wait_for_key()
{
	while (true)
	{
		poll();
		for (size_t i = 0; i < _n_columns*_n_rows; i++)
		{
			if (keys[i]) return i;
		}
	}
}

void Keypad::wait_for_key(uint16_t key)
{
	while(!keys[key]) poll();
}

uint16_t Keypad::wait_for_release()
{
	int32_t key = -1;
	while(true)
	{
		poll();
		bool empty = true;
		for (size_t i = 0; i < _n_columns*_n_rows; i++)
		{
			if (keys[i]) {
				key = i;
				empty = false;
			}
		}
		if (empty && (key != -1)) {
			delay(REBOUND_TIME);
			return key;
		}
	}
}

void Keypad::wait_until_released(uint16_t key)
{
	while(keys[key]) poll();
	delay(REBOUND_TIME);
}

void Keypad::begin() {
	for (size_t i = 0; i < _n_rows; i++)
	{
		pinMode(_row_pins[i], INPUT_PULLUP);
	}
	for (size_t i = 0; i < _n_columns; i++)
	{
		pinMode(_column_pins[i], OUTPUT);
		digitalWrite(_column_pins[i], HIGH); 
	}
}