#pragma once
#include <memory>
#include "token.hpp"

enum Op { NUM, BIN, UNARY, COMPOUND, EQ, VAR, NOOP, PROGRAM, BLOCK, VARDECL, TYPE };

class Node {
    protected:
        Op op;
        std::shared_ptr<Node> left;
        Token token;
        std::shared_ptr<Node> right;

        std::vector<std::shared_ptr<Node>> children;

    public:
         // General one
        Node(Node l, Node r, Token t, Op o) : token(t), op(o) {
            left = std::make_shared<Node>(l);
            right = std::make_shared<Node>(r);
        }

        // For unary operators
        Node(Node expr, Token t, Op o) : right(NULL), token(t), op(o) {
            left = std::make_shared<Node>(expr);
        }

        // For numbers and types
        Node(Token t, Op o) : left(NULL), right(NULL), token(t), op(o) {} // For numbers

        // For compound_statement
        Node(Op o, std::vector<Node> cs) : left(NULL), right(NULL), token(Token(EMPTY, "")), op(o) {
            for(auto c : cs) {
                children.push_back(std::make_shared<Node>(c));
            }
        }

        std::shared_ptr<Node> get_left() { return left; }
        Token get_token() { return token; }
        std::shared_ptr<Node> get_right() { return right; }
        Op get_op() { return op; }
        std::vector<std::shared_ptr<Node>> get_children() { return children; }
};
