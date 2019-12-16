#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector> 
#include <iostream>
#include <unordered_map>  


using std::string;


struct cons_cell_t {

	union car {
		int          Int;
		float        Float;
		std::string* Str;
		cons_cell_t* Car;
	};
	cons_cell_t* cdr;
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

	/*
	printf("\n");
	for(std::vector<std::string>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {
	    // process i
	    std::cout << *i << "\n"; // this will print all the contents of *features*
	}
	*/


	return tokens;
}

int apply_op(std::string &op, int a, int b) {

	if(op == "+") {
		return a + b;
	}
	else if( op == "-") {
		return a - b;
	}
	else if(op == "*") {
		return a * b;
	}
	return a;

}


// let's just try handling some simple operations with any number of expressions first
int eval( std::vector<string> &expression, int start=0 ) {

	/*
	   |
	   v
	[ '(', +, 3, 4, 5 ')' ]

	       |
	       v
	[ '(', +, 3, 4, 5 ')' ]

			  |
	          v
	[ '(', +, 3, 4, 5 ')' ]

	*/

	int index = start + 1;
	

	std::string op = expression[index];
	index++;


	// TODO: make it possible for this to be a sub S-exp too
	int left = std::stoi(expression[index]);
	int right = 0;


	while (index < expression.size()) {

		if( expression[index + 1] == ")" )
			break;
		else if( expression[index + 1] == "(" ) {
			right = eval(expression, index + 1);

			// hacky but it works
			while(expression[index] != ")")
				++index;
			--index;
		}
		else {
			right = std::stoi(expression[index + 1]);
		}

		
		left = apply_op(op, left, right);

		++index;
	}


	//std::cout << left << "_\n";
	return left;
}




int main( int argc, const char* argv[] )
{

	std::string temp = argv[1];
	std::vector<string> expr = tokenize(temp);

	int r = eval(expr);

	std::cout << r << "\n";
}