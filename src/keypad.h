#include <Arduino.h>

class Keypad {
	private:
		uint8_t _n_rows;
		uint8_t _n_columns;
		uint8_t* _row_pins;
		uint8_t* _column_pins;
	public:
		bool* keys;
		Keypad(uint8_t n_rows, uint8_t n_columns, uint8_t* row_pins, uint8_t* column_pins) : _n_rows(n_rows), _n_columns(n_columns), _row_pins(row_pins), _column_pins(column_pins) {
			keys = (bool*)malloc(n_rows*n_columns);
		}
		void begin();
		void poll();
		uint16_t wait_for_key();
		void wait_for_key(uint16_t key);
		uint16_t wait_for_release();
		void wait_until_released(uint16_t key);
};