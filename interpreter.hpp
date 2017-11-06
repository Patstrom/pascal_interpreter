#pragma once
#include <memory>
#include "ast.hpp"
#include "token.hpp"
#include "symbol_table_builder.hpp"

#include <stdio.h>
#include <string>
#include <map>

typedef std::shared_ptr<Node> node_ptr;

class Interpreter {
    map<string, double> GLOBAL_MEM;

    private:
        node_ptr tree;

        double visit_unary(node_ptr n) {
            switch(n->get_token().get_type()) {
                case PLUS:
                    return + visit(n->get_left());
                case MINUS:
                    return - visit(n->get_left());
            }
        }

        double visit_num(node_ptr n) {
            return n->get_token().get_double();
        }

        double visit_binop(node_ptr n) {
            switch(n->get_token().get_type()) {
                case PLUS:
                    return visit(n->get_left()) + visit(n->get_right());
                case MINUS:
                    return visit(n->get_left()) - visit(n->get_right());
                case MUL:
                    return visit(n->get_left()) * visit(n->get_right());
                case INTEGER_DIV:
                    // Cast to int to round it and then cast to double to be able to return
                    return (double) ( (int)visit(n->get_left()) / visit(n->get_right()) );
                case FLOAT_DIV:
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

        double visit_assign(node_ptr n) {
            std::string var_name = n->get_left()->get_token().get_value();
            double value = visit(n->get_right());
            GLOBAL_MEM[var_name] = value;
            return value;
        }


        double visit_var(node_ptr n) {
            std::string var_name = n->get_token().get_value();
            if ( GLOBAL_MEM.find(var_name) != GLOBAL_MEM.end() ) {
                return GLOBAL_MEM[var_name];
            } else {
                cout << var_name << " was not declared" << endl;
                exit(1);
            }

        }

        void visit_program(node_ptr n) {
            visit(n->get_left());
        }

        void visit_block(node_ptr n) {
            for(auto ptr : n->get_children()) {
                visit(ptr);
            }
        }

        void visit_vardecl(node_ptr n) {
            return;
        }

        void visit_type(node_ptr n) {
            return;
        }

        double visit(node_ptr n) {
            switch(n->get_op()) {
                case NUM:
                    return visit_num(n);
                case BIN:
                    return visit_binop(n);
                case UNARY:
                    return visit_unary(n);
                case COMPOUND:
                     visit_compound(n);
                     break;
                case EQ:
                    return visit_assign(n);
                case VAR:
                    return visit_var(n);
                case NOOP:
                    visit_noop(n);
                    break;
                case PROGRAM:
                    visit_program(n);
                    break;
                case BLOCK:
                    visit_block(n);
                    break;
                case VARDECL:
                    visit_vardecl(n);
                    break;
                case TYPE:
                    visit_type(n);
                    break;
            }
            return 0;
        }

    public:
    Interpreter(node_ptr tree) : tree(tree) {}
    void interpret() { 
        SymbolTableBuilder stb;
        stb.visit(tree);
        visit(tree);
        std::cout << "GLOBAL_MEM: {" << std::endl;
        for(auto elem : GLOBAL_MEM) {
            std::cout << "\t" << elem.first << " : " << elem.second << std::endl;
        }
        std::cout << "}" << std::endl;
        std::cout << stb.get_symbol_table() << std::endl;
    }

};
