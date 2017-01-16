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

	void command() {
		if ()
		eat(INTEGER);
		eat(DOT);
	}

	void loop() {
		eat("REP");
	}

public:
	Interpreter() : lexer{ lexer }, current{ lexer.get_next_token() } {
	}

};