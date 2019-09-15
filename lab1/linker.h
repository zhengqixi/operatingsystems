#include "SymbolTable.h"
#include "parser.h"
#include <ostream>
#include <string>
#include <utility>
namespace NYU {
namespace OperatingSystems {
    class Linker {
    public:
        Linker(std::string fileName, std::ostream& output);
        void link();

    private:
        std::string d_fileName;
        std::ostream& d_output;
        // Pass 1
        SymbolTable generateSymbolTable();
        // Pass 2
        void generateLinkedFile(const SymbolTable& symTable);
        std::pair<std::string, int> getSymbol(Parser& parser) const;
        std::string parseIgnoreEmpty(Parser& parser) const;
    };
} // close namespace OperatingSystems
} // close namespace NYu