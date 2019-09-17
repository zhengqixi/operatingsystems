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
        // Gets a symbol, which is a string followed by a integer
        // Can also be used for address
        std::pair<std::string, int> getSymbol(Parser& parser) const;
        // Get the next non-empty token
        std::string parseIgnoreEmpty(Parser& parser) const;
        // Get the size of the define section
        int getDefineSize(Parser& parse);
        // Helper function for 0 padding the total width of a numerical output to totalWidth
        std::string padZeroOutput(int output, int totalWidth);
    };
} // close namespace OperatingSystems
} // close namespace NYu