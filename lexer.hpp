#pragma once
#include "token.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

class Lexer {
private:
	string text;
	char current_char;
	int pos;
	int len;
	int line_number = 1;

    map<string, Token> reserved_keywords = {
        make_pair ("BEGIN", Token(BEGIN, "BEGIN")),
        make_pair ("END", Token(END, "END"))
    };


	void advance() {
		pos++;
		if (pos >= len)
			current_char = '\0';
		else current_char = text[pos];
	}

    char peek() {
       int peek_pos = pos + 1;
       if (peek_pos > len - 1) {
            return 0;
       } else {
            return text[peek_pos];
       }
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

	Token read_word() {
		string result = "";
		while (isalpha(current_char) && current_char != 0) {
			result += current_char;
			advance();
		}

        // Word is reserved
        if ( reserved_keywords.find(result) != reserved_keywords.end()) {
            return reserved_keywords[result];
        } else {
            return Token(ID, result);
        }
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

            if (current_char == ';') {
                advance();
                return Token(SEMI, ";");
            }

            if (current_char == '.') {
                advance();
                return Token(DOT, ".");
            }

            if (current_char == ':' && peek() == '=') {
                advance();
                advance();
                return Token(EQ, ":=");
            }

            // Single character token are exhausted. Must be a word
            if( isalpha(current_char) ) {
                return read_word(); 
            }


			error();
		}

		// EOF

		return Token($, "$");
	}
};
