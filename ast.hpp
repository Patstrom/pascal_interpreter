#pragma once
#pragma once
#include <memory>
#include "token.hpp"

enum Op { NUM, BIN, UNARY };

class Node {
    protected:
        Op op;
        std::shared_ptr<Node> left;
        Token token;
        std::shared_ptr<Node> right;

    public:
        Node(Node l, Node r, Token t, Op o) : token(t), op(o) {
            left = std::make_shared<Node>(l);
            right = std::make_shared<Node>(r);
        } // General one
        Node(Node expr, Token t, Op o) : right(NULL), token(t), op(o) {
            left = std::make_shared<Node>(expr);
        } // For unary operators
        Node(Token t, Op o) : left(NULL), right(NULL), token(t), op(o) {} // For numbers

        std::shared_ptr<Node> get_left() { return left; }
        Token get_token() { return token; }
        std::shared_ptr<Node> get_right() { return right; }
        Op get_op() { return op; }
};
