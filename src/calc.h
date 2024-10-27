#include <string>
#include "tinyexpr.h"

class Expression {
	public:
		std::string text = "";
		int16_t cursor = 0;
		int error = 0;
		double result = 0.0;

		void eval();
		void write(const std::string &input);
		void remove(uint16_t n);
		void draw();
		void clear();
};


const char* const mod_names[8] = {"s"};
enum Modifier : uint8_t {
	MOD_SHIFT = 0b1,
};

extern Expression curr_expr;