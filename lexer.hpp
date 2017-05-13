#pragma once
#include "token.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Lexer {
private:
	string text;
	char current_char;
	int pos;
	int len;
	int line_number = 1;


	void advance() {
		pos++;
		if (pos >= len)
			current_char = '\0';
		else current_char = text[pos];
	}

	void error() {
		cout << "Syntaxerror on line " << line_number << endl;
		exit(0);
	}

	void skip_whitespace() {
		while (isspace(current_char) && current_char != 0) {
			if (current_char == '\n') line_number++;
			advance();
		}
	}

	void skip_comment() {
		while (current_char != '\n' && current_char != 0) advance();
		line_number++;
	}

	string read_integer() {
		string result = "";
		while (isdigit(current_char) && current_char != 0) {
			result += current_char;
			advance();
		}
		return result;
	}

	string read_word() {
		string result = "";
		while (isalpha(current_char) && current_char != 0) {
			result += current_char;
			advance();
		}

		return result;
	}

	bool is_in(vector<string> & vector,  string value) {
		transform(value.begin(), value.end(), value.begin(), ::tolower); // Make lowercase
		return find(vector.begin(), vector.end(), value) != vector.end();
	}

public:
	Lexer(string t) {
		text = t;
		pos = 0;
		len = text.length();
		current_char = text[pos];

	}

	Token get_next_token() {
		while (current_char != '\0') {
			
			// Skip spaces
			if (isspace(current_char)) {
				this->skip_whitespace();
				continue;
			}

			// Skip comments
			if (current_char == '%') {
				this->skip_comment();
				continue;
			}

			// Read an integer
			if (isdigit(current_char)) {
				return Token(INTEGER, this->read_integer());
			}

			if (current_char == '+') {
				advance();
				return Token(PLUS, "+");
			}

			if (current_char == '-') {
				advance();
				return Token(MINUS, "-");
			}

			if (current_char == '*') {
				advance();
				return Token(MUL, "*");
			}

			if (current_char == '/') {
				advance();
				return Token(DIV, "/");
			}

			if (current_char == '(') {
				advance();
				return Token(LPAREN, "(");
			}

			if (current_char == ')') {
				advance();
				return Token(RPAREN, ")");
			}
			//string word = this->read_word();
			//if (is_in(valid_commands, word)) {
			//		return Token(COMMAND, word);
			//}

			error();
		}

		// EOF

		return Token($, "$");
	}
};
