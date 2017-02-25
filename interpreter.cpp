#include "lexer.cpp"

class Interpreter {
private:
	Lexer lexer;
	Token current;

	void error() {
		//TODO fix this shiet
		cout << "Syntaxfel på rad " << endl;
	}

	void eat(Type expected_type) {
		if (current.get_type() == expected_type) {
			current = lexer.get_next_token();
		}
		else {
			error();
		}
	}

    // factor: INTEGER | LPAREN expr RPAREN
    int factor() {
        Token token = current;
        if(token.get_type() == INTEGER) {
            eat(INTEGER);
            return token.get_int();
        }
        if(token.get_type() == LPAREN) {
            eat(LPAREN);
            int result = expr();
            eat(RPAREN);
            return result;
        }
    }

    // term: factor ((MUL|DIV) factor)*
    int term() {
        int result = factor();

        while(current.get_type() == MUL || current.get_type() == DIV) {
            Token token = current;
            if(token.get_type() == MUL) {
                eat(MUL);
                result = result * factor();
            } 
             if(token.get_type() == DIV) {
                eat(DIV);
                result = result / factor();
            }
        }
        
        return result;
    }

    // expr : term ((PLUS|MINUS) term)*
    int expr() {
        int result = term();

        while(current.get_type() == PLUS || current.get_type() == MINUS) {
            Token token = current;
            if(token.get_type() == PLUS) {
                eat(PLUS);
                result = result + term();
            } else if(token.get_type() == MINUS) {
                eat(MINUS);
                result = result - term();
            }
        }

        return result;
    }

public:
	Interpreter(Lexer l) : lexer{ l }, current{ lexer.get_next_token() } {
	}

    int start() { return expr(); }

};
