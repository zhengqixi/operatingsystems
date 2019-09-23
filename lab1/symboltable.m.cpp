#include "SymbolTable.h"
#include <iostream>

int main()
{
    using namespace std;
    using namespace NYU::OperatingSystems;
    SymbolTable symMe;
    symMe.addSymbol("bigsad", 12, 1);
    symMe.addSymbol("niceMe", 13, 2);
    symMe.addSymbol("omgu", 43, 3);
    cout << symMe.addSymbol("bigsad", 33, 9999) << endl;
    cout << symMe;
    for (const auto& sym : symMe.allSymbols()) {
        cout << sym.symbol << " at " << sym.absoluteAddress << " and module: " << sym.moduleNumber << '\n';
    }
    return 0;
}