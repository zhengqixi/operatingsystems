#include "SymbolTable.h"
#include <iostream>
namespace NYU {
namespace OperatingSystems {
    class Linker {
    public:
        Linker(std::ifstream& input, std::ostream& output);
        void link();

    private:
        std::ifstream& d_input;
        std::ostream& d_output;
        // Pass 1
        SymbolTable generateSymbolTable();
        // Pass 2
        void generateLinkedFile(const SymbolTable& symTable);
    };
} // close namespace OperatingSystems
} // close namespace NYu