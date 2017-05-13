#include <memory>
#include "ast.hpp"
#include "token.hpp"

#include <stdio.h>

typedef std::shared_ptr<Node> node_ptr;

class Interpreter {

    private:
        node_ptr tree;

        int visit_unary(node_ptr n) {
            switch(n->get_token().get_type()) {
                case PLUS:
                    return + visit(n->get_left());
                case MINUS:
                    return - visit(n->get_left());
            }
        }

        int visit_num(node_ptr n) {
            return n->get_token().get_int();
        }

        int visit_binop(node_ptr n) {
            switch(n->get_token().get_type()) {
                case PLUS:
                    return visit(n->get_left()) + visit(n->get_right());
                case MINUS:
                    return visit(n->get_left()) - visit(n->get_right());
                case MUL:
                    return visit(n->get_left()) * visit(n->get_right());
                case DIV:
                    return visit(n->get_left()) / visit(n->get_right());
            }
        }

        int visit(node_ptr n) {
            switch(n->get_op()) {
                case NUM:
                    return visit_num(n);
                case BIN:
                    return visit_binop(n);
                case UNARY:
                    return visit_unary(n);
            }
        }

    public:
    Interpreter(node_ptr tree) : tree(tree) {}
    int interpret() { return visit(tree); }
};
