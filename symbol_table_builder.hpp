#pragma once
#include "symbol_table.hpp"
#include <memory>

typedef std::shared_ptr<Node> node_ptr;

 class SymbolTableBuilder {
    private:
        SymbolTable symbols;
    protected:
        void visit_unary(node_ptr n) {
            visit(n->get_left());
        }

        void visit_num(node_ptr n) {
            return;
        }

        void visit_binop(node_ptr n) {
            visit(n->get_left());
            visit(n->get_right());
        }

        void visit_compound(node_ptr n) {
            for(auto ptr: n->get_children()) {
                visit(ptr);
            }
        }

        void visit_noop(node_ptr n) {
            return;
        }

        void visit_program(node_ptr n) {
            visit(n->get_left());
        }

        void visit_block(node_ptr n) {
            for(auto ptr: n->get_children()) {
                visit(ptr);
            }
        }

        void visit_vardecl(node_ptr n) {
            string type_name = n->get_right()->get_token().get_value();
            Symbol type_symbol = symbols.lookup(type_name);
            string var_name = n->get_left()->get_token().get_value();
            VarSymbol var_symbol(var_name, type_symbol);
            symbols.define(var_symbol);
        }

        void visit_assign(node_ptr n) {
            string var_name = n->get_left()->get_token().get_value();

            Symbol var_symbol = symbols.lookup(var_name);
            if ( var_symbol.get_name() == "" && var_symbol.get_type() == nullptr ) {
                cout << var_name << " is not defined" << endl;
                exit(1);
            }
        }

        void visit_var(node_ptr n) {
            string var_name = n->get_token().get_value();

            Symbol var_symbol = symbols.lookup(var_name);
            if ( var_symbol.get_name() == "" && var_symbol.get_type() == nullptr ) {
                cout << "Variable is not defined" << endl;
                exit(1); 
            }
        }

    public:
        SymbolTableBuilder() : symbols(SymbolTable()) {}
        void visit(node_ptr n) {
            switch(n->get_op()) {
                case NUM:
                    visit_num(n);
                    break;
                case BIN:
                     visit_binop(n);
                     break;
                case UNARY:
                     visit_unary(n);
                     break;
                case COMPOUND:
                     visit_compound(n);
                     break;
                case EQ:
                     visit_assign(n);
                     break;
                case VAR:
                     visit_var(n);
                     break;
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
                //case TYPE:
                //    visit_type(n);
                //    break;
            }

        }

        const SymbolTable& get_symbol_table() const { return symbols; }
};
