#include "lexer.hpp"
#include "ast.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include <iostream>
#include <fstream>

void printTree(std::shared_ptr<Node> n) {
    if(n == NULL) return;
    Token t = n->get_token();
    std::cout << n->get_op() << " : " << t.get_type() << " : " << t.get_value() << std::endl;
    if(n->get_op() == COMPOUND || n->get_op() == BLOCK) {
        std::cout << "Number of statements: " << n->get_children().size() << std::endl;
        for(auto child : n->get_children()) {
            printTree(child);
        }
        std::cout << "exit" << std::endl;
    } else {
        printTree(n->get_left());
        printTree(n->get_right());
    }
}


int main(int argc, char * argv[])
{
    std::ifstream infile(argv[1]);

    std::string program((std::istreambuf_iterator<char>(infile)),
                     std::istreambuf_iterator<char>());
    infile.close();

    Lexer lexer(program);
    Parser parser(lexer);
    auto root = parser.parse();
    //printTree(root);
    Interpreter interpreter(root);

    interpreter.interpret();
	
	return 0;
}

