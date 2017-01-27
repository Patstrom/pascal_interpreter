#include "lexer.cpp"

class Interpreter {
private:
	Lexer lexer;
	Token current;

	int x, y, direction;
	string color;

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

	// COMMAND_LIST -> INTRUCTION COMMAND_LIST
	void command_list() {
		while (current.get_type() == COMMAND && current.get_type() != $) {
			instruction();
		}
	}

	// INSTRUCTION -> COMMAND INTEGER DOT | COMMAND DOT
	// COMMAND -> FORW | BACK | LEFT | RIGHT | UP | DOWN | COLOR | REP
	void instruction() {
		string command = current.get_value();
		transform(command.begin(), command.end(), command.begin(), ::tolower);
		eat(COMMAND);

		if (command == "forw") {
			int magnitude = std::stoi(current.get_value());
			eat(INTEGER);
			eat(DOT);
		}

		if (command == "back") {
			int magnitude = std::stoi(current.get_value());
			eat(INTEGER);
			eat(DOT);
		}

		if (command == "left") {
			int magnitude = std::stoi(current.get_value());
			eat(INTEGER);
			eat(DOT);
		}

		if (command == "right") {
			int magnitude = std::stoi(current.get_value());
			eat(INTEGER);
			eat(DOT);
		}

		if (command == "color") {
			string color = current.get_value();
			eat(HEX);
			eat(DOT);
		}

		if (command == "rep") {
			loop();
		}

		if (command == "up") {
			eat(DOT);
		}

		if (command == "down") {
			eat(DOT);
		}

	}

	// loop -> REP QUOT COMMAND_LIST QUOT | REP INSTRUCTION
	void loop() {
		int magnitude = std::stoi(current.get_value());
		eat(INTEGER);

		if (current.get_type() == QUOT) {
			eat(QUOT);
			//TODO FIX THIS. DOESN'T REPEAT THE SAME COMMAND
			while (magnitude-- > 0) {
				instruction();
			}
			eat(QUOT);
		}
		else {
			//TODO SAME HERE
			while (magnitude-- > 0) {
				instruction();
			}
		}
	}

public:
	Interpreter() : lexer{ lexer }, current{ lexer.get_next_token() } {
	}

};