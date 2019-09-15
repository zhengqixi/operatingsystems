#include "SymbolTable.h"
#include <iostream>
namespace NYU {
namespace OperatingSystems {
    class Linker {
    public:
        void link(std::ifstream& input, std::ostream& output);

    private:
        // Pass 1
        SymbolTable generateSymbolTable(std::ifstream& input);
        // Pass 2
        void generateLinkedFile(std::ostream& output);
    };
} // close namespace OperatingSystems
} // close namespace NYu