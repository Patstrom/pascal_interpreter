#pragma once
#include "ast.hpp"
#include "lexer.hpp"

class Parser {
private:
	Lexer lexer;
	Token current;

    void error() {
        cout << "Error parsing" << endl;
        exit(0);
    }

	void error(Type expected_type) {
		cout << "Expected type " << expected_type << endl;
        error();
	}

	void eat(Type expected_type) {
		if (current.get_type() == expected_type) {
			current = lexer.get_next_token();
		}
		else {
			error(expected_type);
			current = lexer.get_next_token();
		}
	}

    // factor: (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN
    Node factor() {
        Token token = current;
        switch(token.get_type()) {
            case PLUS:
                eat(PLUS);
                return Node(factor(), token, UNARY);
            case MINUS:
                eat(MINUS);
                return Node(factor(), token, UNARY);
            case INTEGER:
                eat(INTEGER);
                return Node(token, NUM);
            case LPAREN:
                eat(LPAREN);
                Node n = expr();
                eat(RPAREN);
                return n;
        }

        error();
    }

    // term: factor ((MUL|DIV) factor)*
    Node term() {
        Node n = factor();

        while(current.get_type() == MUL || current.get_type() == DIV) {
            Token token = current;
            if(token.get_type() == MUL) {
                eat(MUL);
            } 
             if(token.get_type() == DIV) {
                eat(DIV);
            }

            n = Node(n, factor(), token, BIN);
        }
        
        return n;
    }

    // expr : term ((PLUS|MINUS) term)*
    Node expr() {
        Node n = term();

        while(current.get_type() == PLUS || current.get_type() == MINUS) {
            Token token = current;
            if(token.get_type() == PLUS) {
                eat(PLUS);
            } else if(token.get_type() == MINUS) {
                eat(MINUS);
            }

            n = Node(n, term(), token, BIN);
        }

        return n;
    }

public:
	Parser(Lexer l) : lexer{ l }, current{ lexer.get_next_token() } {
	}

    std::shared_ptr<Node> parse() { return std::make_shared<Node>(expr()); }

};
