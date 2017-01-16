#include "lexer.cpp"
#include <iostream>
#include <fstream>

int main(int argc, char * argv[])
{
	while (1) {
		std::string text = "";

		std::getline(std::cin, text);

		if (text.empty()) break;

		Lexer lexer = Lexer(text);
		
		for (auto t = lexer.get_next_token(); t.get_type() != $; t = lexer.get_next_token())
			std::cout << t.get_type() << " : " << t.get_value() << endl;
	}
	
	return 0;
}

