#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector> 
#include <iostream> 


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

	std::cout << chars << "\n";

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

	printf("\n");
	for(std::vector<std::string>::const_iterator i = tokens.begin(); i != tokens.end(); ++i) {
	    // process i
	    std::cout << *i << "\n"; // this will print all the contents of *features*
	}	 

	return tokens;
}





int main( int argc, const char* argv[] )
{

	std::string temp = argv[1];
	tokenize(temp);
}