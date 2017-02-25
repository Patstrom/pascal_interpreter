#pragma once
#include <string>

enum Type { $, INTEGER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN };

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
};
