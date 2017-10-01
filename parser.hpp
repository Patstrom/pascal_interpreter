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
        cout << "Current token: " << current.get_type() << " : " << current.get_value() << endl;
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

    // factor: (PLUS | MINUS) factor
    //         | INTEGER_CONST
    //         | REAL_CONST
    //         | LPAREN expr RPAREN 
    //         | variable
    Node factor() {
        Token token = current;
        switch(token.get_type()) {
            case PLUS:
                eat(PLUS);
                return Node(factor(), token, UNARY);
            case MINUS:
                eat(MINUS);
                return Node(factor(), token, UNARY);
            case INTEGER_CONST:
                eat(INTEGER_CONST);
                return Node(token, NUM);
            case REAL_CONST:
                eat(REAL_CONST);
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

    // term: factor ((MUL| INTEGER_DIV| FLOAT_DIV) factor)*
    Node term() {
        Node n = factor();

        while(current.get_type() == MUL || current.get_type() == INTEGER_DIV 
                || current.get_type() == FLOAT_DIV) {
            Token token = current;
            if(token.get_type() == MUL) {
                eat(MUL);
            } 
            if(token.get_type() == INTEGER_DIV) {
                eat(INTEGER_DIV);
            }
            if(token.get_type() == FLOAT_DIV) {
                eat(FLOAT_DIV);
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

    // program : PROG variable SEMI block DOT
    Node program() {
        eat(PROG);
        Node var_node = variable();
        string prog_name = var_node.get_token().get_value();
        eat(SEMI);
        
        Node block_node = block();
        Node program_node(block_node, Token(PROG, prog_name), PROGRAM);
        eat(DOT);
        return program_node;
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

    // block: declarations compound_statement
    Node block() {
        vector<Node> nodes = declarations();
        nodes.push_back(compound_statement()); // compount_statement will still be run last after the declaration nodes.
        return Node(BLOCK, nodes);
    }

    // declarations: VARIABLE (variable_declarations SEMI)+
    //               | empty
    vector<Node> declarations() {
        vector<Node> results;
        
        if (current.get_type() == VARIABLE) {
            eat(VARIABLE);
            while (current.get_type() == ID) {
                vector<Node> n = variable_declaration();
                results.insert(end(results), begin(n), end(n));
                eat(SEMI);
            }
        }

        return results;
    }

    // variable_declaration : ID (COMMA ID)* COLON type_spec
    vector<Node> variable_declaration() {
        vector<Node> var_nodes = { Node(current, VAR) };
        eat(ID);

        while (current.get_type() == COMMA) {
            eat(COMMA);
            var_nodes.push_back( Node(current, VAR) );
            eat(ID);
        }

        eat(COLON);
        
        vector<Node> results;
        Node type_node = type_spec();
        for(auto var : var_nodes) {
            results.push_back(Node(var, type_node, Token(EMPTY, ""), VARDECL ));
        }

        return results;
    }

    // type_spec: INTEGER
    //            | REAL
    Node type_spec() {
        if (current.get_type() == INTEGER) {
            eat(INTEGER);
        } else {
            eat(REAL);
        }
        return Node(current, TYPE);
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
