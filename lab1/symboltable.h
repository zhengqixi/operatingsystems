#include <ostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H
namespace NYU {
namespace OperatingSystems {
    struct Symbol {
    public:
        std::string symbol;
        int absoluteAddress;
        int moduleNumber;
        Symbol(std::string symbol, int absoluteAddress, int moduleNumber)
            : symbol(symbol)
            , absoluteAddress(absoluteAddress)
            , moduleNumber(moduleNumber)
        {
        }
    };
    class SymbolTable {
    public:
        // Adds a new symbol to the symbol table. Returns true if added succesfully. False if the symbol already exists
        bool addSymbol(std::string symbol, int absolutePosition, int moduleNumber);
        // Looks up a symbol and returns its address.
        // If the symbol does not exist, -1 is returned
        int getSymbol(const std::string& symbol) const;
        // Gets ALL the symbols inserted.
        const std::vector<Symbol>& allSymbols() const;
        // Ostream& overload to print
        friend std::ostream& operator<<(std::ostream& os, const SymbolTable& table);

    private:
        // Map from the symbol to its position globally
        std::unordered_map<std::string, int> d_symbolMap;
        // Vector of symbol objects for printing linearlly at the end
        std::vector<Symbol> d_symbols;
        // Set to store whether or not a symbol has been duplicatly defined
        std::unordered_set<std::string> d_doublyDefined;
    };
} // close namespace OperatingSystems
} // close namespace NYU
#endif