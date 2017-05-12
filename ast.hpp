#pragma once
#pragma once
#include <memory>
#include "token.hpp"

class Node {
    protected:
        std::shared_ptr<Node> left;
        Token token;
        std::shared_ptr<Node> right;
    public:
        Node(Node l, Token t, Node r) : token(t) {
            left = std::make_shared<Node>(l);
            right = std::make_shared<Node>(r);
        }
        
        Node(Token t) : token(t), left(NULL), right(NULL) {}

        std::shared_ptr<Node> get_left() { return left; }
        Token get_token() { return token; }
        std::shared_ptr<Node> get_right() { return right; }
};

class num : public Node {
    public:
        num(Token t) : Node(t) {}
};

class binop : public Node {
    public:
        binop(Node l, Token op, Node r) : Node(l, op, r) { }
};
