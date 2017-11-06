#pragma once
#include <string>
#include <map>
#include <memory>

using namespace std;

class Symbol {
    protected:
        string name;
        std::shared_ptr<Symbol> type;
    public:
        Symbol() : name(""), type(nullptr) {}
        Symbol(string n) : name(n), type(nullptr) {}
        Symbol(string n, Symbol t) : name(n){
            type = std::make_shared<Symbol>(t);
        }

        string get_name() { return name; }
        std::shared_ptr<Symbol> get_type() { return type; }
};

class BuiltinType : public Symbol {
    public:
        BuiltinType(string n) : Symbol(n) {}
};

class VarSymbol : public Symbol {
    public:
        VarSymbol(string n, Symbol t) : Symbol(n, t) {}
};

class SymbolTable {
    private:
        map<string, Symbol> symbols;

        void init_builtins() {
            this->insert(BuiltinType("INTEGER"));
            this->insert(BuiltinType("REAL"));
        }
    public:
        SymbolTable() {
            init_builtins();
        }

        void insert(Symbol s) {
            symbols[s.get_name()] = s;
        }

        Symbol find(string name) { return symbols[name]; }

        friend std::ostream& operator<<(std::ostream& strm, const SymbolTable &s) {
            strm << "symbol_table: {" << std::endl;
            for(auto elem : s.symbols) {
                strm << "\t" << elem.first << " : " << elem.second.get_name() << std::endl;
            }
            return strm << "}";
        }
};
