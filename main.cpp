#include "lexer.hpp"
#include "ast.hpp"
#include "parser.hpp"
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
    std::ifstream infile(argv[1]);

    std::string line;
    while(std::getline(infile, line)) {
        Lexer lexer(line);
        Parser parser(lexer);
        Interpreter interpreter(parser.parse());

        std::cout << interpreter.interpret() << std::endl;
    }
	
	return 0;
}

