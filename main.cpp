#include "lexer.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "interpreter.hpp"
#include <iostream>
#include <fstream>

void printTree(std::shared_ptr<Node> n) {
    if(n == NULL) return;
    Token t = n->get_token();
    std::cout << t.get_type() << " : " << t.get_value() << std::endl;
    printTree(n->get_left());
    printTree(n->get_right());
}


int main(int argc, char * argv[])
{
    for(std::string line; std::getline(std::cin, line);) {
        if(line.empty()) continue;
        Lexer lexer(line);
        Parser parser(lexer);
        Interpreter interpreter(parser.parse());

        std::cout << interpreter.interpret() << std::endl;
    }
//	while (1) {
//		std::string text = "";
//
//		std::getline(std::cin, text);
//
//		if (text.empty()) break;
//
//		Lexer lexer = Lexer(text);
//		
//		for (auto t = lexer.get_next_token(); t.get_type() != $; t = lexer.get_next_token())
//			std::cout << t.get_type() << " : " << t.get_value() << endl;
//	}
	
	return 0;
}

