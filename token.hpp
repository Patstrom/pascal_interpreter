#pragma once
#include <string>

enum Type { $, EMPTY, NUMBER, PLUS, MINUS, MUL, LPAREN, RPAREN, BEGIN, END, DOT, ID, ASSIGN, SEMI, PROGRAM, COLON, COMMA, INTEGER, REAL, INTEGER_CONST, REAL_CONST, INTEGER_DIV, FLOAT_DIV };

class Token
{
private:
	Type t;
	std::string v;
public:
	// Again with the initializer lists... Damn you Microsoft
	Token(Type token, std::string value) { t = token; v = value; }
	Type get_type() { return t; }
	std::string get_value() { return v; }
    int get_int() { return std::stoi(v); }

    // Default contructor so we can use the map<string, Token> for reserved keywords
    Token() { t = $; v = ""; }
};
