#pragma once
#include "ast.hpp"
#include "lexer.hpp"
#include <iostream>

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

    // factor: (PLUS | MINUS) factor | INTEGER | LPAREN expr RPAREN | VAR
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
                {
                    eat(LPAREN);
                    Node n = expr();
                    eat(RPAREN);
                    return n;
                }
            default:
                return variable();
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

    // program : compount_statement DOT
    Node program() {
        Node n = compound_statement();
        eat(DOT);
        return n;
    }

    // compound_statement: BEGIN statement_list END
    Node compound_statement() {
        eat(BEGIN);
        std::vector<Node> nodes = statement_list();
        eat(END);

        Node n(COMPOUND, nodes);
        return n;
    }

    // statement_list: statement
    //                 | statement SEMI statement_list
    vector<Node> statement_list() {
        Node n = statement();

        vector<Node> results = { n };

        while (current.get_type() == SEMI) {
            eat(SEMI);
            results.push_back(statement());
        }

        if (current.get_type() == ID) {
            error();
        }

        return results;
    }

    // statement: compound_statement
    //            | assignment_statement
    //            | empty
    Node statement() {
        if (current.get_type() == BEGIN) {
            return compound_statement();
        }
        else if (current.get_type() == ID) {
            return assignment_statement();
        } else {
            return empty();
        }
    }

    // assignment_statement: variable ASSIGN expr
    Node assignment_statement() {
        Node left = variable();
        Token token = current;
        eat(ASSIGN);
        Node right = expr();

        return Node(left, right, token, EQ);
    }

    // variable: ID
    Node variable() {
        Token token = current;
        eat(ID);
        return Node(token, VAR);
    }

    // empty: 
    Node empty() {
        return Node(Token(EMPTY, ""), NOOP);
    }

public:
	Parser(Lexer l) : lexer{ l }, current{ lexer.get_next_token() } {
	}

    std::shared_ptr<Node> parse() {
        Node n = program();
        if (current.get_type() != $) error();

        return std::make_shared<Node>(n);
    }

};
