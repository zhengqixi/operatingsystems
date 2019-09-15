#include <ostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
namespace NYU {
namespace OperatingSystems {
    class SymbolTable {
    public:
        // Adds a new symbol to the symbol table. Returns true if added succesfully. False if the symbol already exists
        bool addSymbol(std::string symbol, int absolutePosition);
        // Looks up a symbol and returns its address.
        // If the symbol does not exist, -1 is returned
        int getSymbol(std::string symbol) const;
        // Ostream& overload to print
        friend std::ostream& operator<<(std::ostream& os, const SymbolTable& table);

    private:
        struct Symbol {
        public:
            std::string symbol;
            int absoluteAddress;
            Symbol(std::string symbol, int absoluteAddress)
                : symbol(symbol)
                , absoluteAddress(absoluteAddress)
            {
            }
        };
        // Map from the symbol to its position globally
        std::unordered_map<std::string, int> d_symbolMap;
        std::vector<Symbol> d_symbols;
    };
} // close namespace OperatingSystems
} // close namespace NYU
#endif