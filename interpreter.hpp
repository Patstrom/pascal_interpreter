#include <memory>
#include "ast.hpp"
#include "token.hpp"

#include <stdio.h>
#include <string>

typedef std::shared_ptr<Node> node_ptr;

class Interpreter {
    std::map<std::string, int> symbol_table;

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

        void visit_compound(node_ptr n) {
            for(auto ptr : n->get_children()) {
                visit(ptr);
            }
        }

        void visit_noop(node_ptr n) {
            return;
        }

        void visit_assign(node_ptr n) {
            std::string var_name = n->get_left()->get_token().get_value();
            symbol_table[var_name] = visit(n->get_right());
        }

        int visit_var(node_ptr n) {
            std::string var_name = n->get_token().get_value();
            if ( symbol_table.find(var_name) != symbol_table.end() ) {
                return 0;
            } else {
                return symbol_table["var_name"];
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
                case COMPOUND:
                    visit_compound(n);
                case ASSIGN:
                    visit_assign(n);
                case VAR:
                    visit_var(n);
                case NOOP:
                    visit_noop(n);
            }
        }

    public:
    Interpreter(node_ptr tree) : tree(tree) {}
    int interpret() { return visit(tree); }
};
