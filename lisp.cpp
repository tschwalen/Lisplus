#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector> 
#include <iostream>
#include <unordered_map>  


using std::string;


struct cons_cell_t {

	union car_t {
		int          Int;
		float        Float;
		std::string* Str;
		cons_cell_t* Car;
	} car;
	cons_cell_t* cdr;
};


struct numeric_t {

    enum{INT, FLOAT} Tag;
    union {
        int Int;
        double Float;
    };
};


/*
 * given a string, returns a vector of strings representing the lisp-tokenization of that string
 *
 */
std::vector<std::string> tokenize( std::string &chars ) {

	std::vector<std::string> tokens;

	//std::cout << chars << "\n";

	int start = 0;
	int end = 0;
	for (std::string::size_type i = 0; i < chars.size(); i++) {
		char current = chars[i];

		if(isspace(current)) {
			if(start != i) {
				tokens.push_back(chars.substr(start, i - start));
			}
			start = i + 1;
		}
		else if (current == '(') {
			if(start != i) {
				tokens.push_back(chars.substr(start, i - start));
			}
			std::string lparen = "(";
			tokens.push_back(lparen);
			start = i + 1;
		}
		else if (current == ')') {
			if(start != i) {
				tokens.push_back(chars.substr(start, i - start));
			}
			std::string rparen = ")";
			tokens.push_back(rparen);
			start = i + 1;
		}

		// collect the rest if the end is reached
		if(i == chars.size() - 1) {
			if(start != i) {
				tokens.push_back(chars.substr(start, i + 1));
			}
		}
	}

	return tokens;
}

float apply_op(std::string &op, float a, float b) {

	if (op == "+") {
		return a + b;
	}
	else if (op == "-") {
		return a - b;
	}
	else if (op == "*") {
		return a * b;
	}
	else if (op == "/") {
		return a / b;
	}
	return a;
}

int apply_op(std::string &op, int a, int b) {

	if (op == "+") {
		return a + b;
	}
	else if (op == "-") {
		return a - b;
	}
	else if (op == "*") {
		return a * b;
	}
	else if (op == "/") {
		return a / b;
	}
	else if (op == "%") {
		return a % b;
	}
	return a;
}

numeric_t apply_op( std::string &op, numeric_t a, numeric_t b ) {

	numeric_t op_result;

	// the result can only be an int if both operands are ints, otherwise the result has to be a float
	if(a.Tag == numeric_t::INT && b.Tag == numeric_t::INT) {
		op_result.Tag = numeric_t::INT;
		op_result.Int = apply_op(op, a.Int, b.Int);
	}
	else {
		op_result.Tag = numeric_t::FLOAT;

		float fa = a.Tag == numeric_t::FLOAT ? a.Float : a.Int;
		float fb = b.Tag == numeric_t::FLOAT ? b.Float : b.Int;

		op_result.Float = apply_op(op, fa, fb);
	}

	return op_result;
}


// converts the string representation of a integer or float into our numeric union-like object
numeric_t get_numeric_literal( std::string &src ) {

	numeric_t literal;

	// a float literal must contain a decimal point in order to be considered as such
	if(src.find('.') != std::string::npos) {
		literal.Tag = numeric_t::FLOAT;
		literal.Float = std::stof(src);
	}
	// otherwise, it must be an int
	else {
		literal.Tag = numeric_t::INT;
		literal.Int = std::stoi(src);
	}

	return literal;
}



// let's just try handling some simple operations with any number of expressions first
std::pair<numeric_t, int> eval( std::vector<string> &expression, int start=0 ) {


	// if this expression does not start with an lparen, then we just return the 
	// value that we're pointed at
	if(expression[start] != "(" )
		return std::make_pair(get_numeric_literal(expression[start]), start);


	int index = start + 1;
	

	std::string op = expression[index];
	++index;

	std::pair<numeric_t, int> first_result = eval(expression, index);
	index = first_result.second;

	numeric_t left = first_result.first;
	numeric_t right = {numeric_t::INT, 0};


	while (index < expression.size()) {

		if( expression[index + 1] == ")" ){
			++index;
			break;
		}
		else if( expression[index + 1] == "(" ) {
			std::pair<numeric_t, int> result = eval(expression, index + 1);
			right = result.first;
			index = result.second - 1;

		}
		else {
			right = get_numeric_literal(expression[index + 1]);
		}

		
		left = apply_op(op, left, right);

		++index;
	}

	return std::make_pair(left, index);
}




int main( int argc, const char* argv[] )
{

	std::string temp = argv[1];
	std::vector<string> expr = tokenize(temp);

	std::pair<numeric_t, int> r = eval(expr);

	if(r.first.Tag == numeric_t::INT) {
		std::cout << r.first.Int << "\n";
	}
	else {
		std::cout << r.first.Float << "\n";
	}
	
}