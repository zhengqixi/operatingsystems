#include "SymbolTable.h"
#include <ostream>
#include <string>
namespace NYU {
namespace OperatingSystems {
    bool SymbolTable::addSymbol(std::string symbol, int absolutePosition)
    {
        if (d_symbolMap.find(symbol) != d_symbolMap.end()) {
            d_doublyDefined.insert(symbol);
            return false;
        }
        d_symbols.push_back(Symbol(symbol, absolutePosition));
        d_symbolMap[symbol] = absolutePosition;
        return true;
    }

    int SymbolTable::getSymbol(std::string symbol) const
    {
        auto value = d_symbolMap.find(symbol);
        if (value == d_symbolMap.end()) {
            return -1;
        }
        return value->second;
    }
    std::ostream& operator<<(std::ostream& os, const SymbolTable& table)
    {
        os << "Symbol Table\n";
        for (auto const& sym : table.d_symbols) {
            os << sym.symbol << "=" << sym.absoluteAddress;
            if (table.d_doublyDefined.find(sym.symbol) != table.d_doublyDefined.end()) {
                os << " Error: This variable is multiple times defined; first value use";
            }
            os << '\n';
        }
        return os;
    }
} // close namespace OperatingSystems
} // close namespace NYU