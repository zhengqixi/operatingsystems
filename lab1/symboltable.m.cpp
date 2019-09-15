#include "SymbolTable.h"
#include <iostream>

int main()
{
    using namespace std;
    using namespace NYU::OperatingSystems;
    SymbolTable symMe;
    symMe.addSymbol("bigsad", 12);
    symMe.addSymbol("niceMe", 13);
    symMe.addSymbol("omgu", 43);
    cout << symMe.getSymbol("bigsad") << endl;
    cout << symMe.getSymbol("saddusme") << endl;
    cout << symMe.addSymbol("bigsad", 33) << endl;
    cout << symMe;
    return 0;
}