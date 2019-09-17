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
        const static int d_maxDefSize = 16;
        const static int d_maxUseSize = 16;
        const static int d_maxIntrSize = 512;
        enum ErrorCode { NUM_EXPECTED,
            SYM_EXPECTED,
            ADDR_EXPECTED,
            SYM_TOO_LONG,
            TOO_MANY_DEF_IN_MODULE,
            TOO_MANY_USE_IN_MODULE,
            TOO_MANY_INSTR };
        std::string d_fileName;
        std::ostream& d_output;
        SymbolTable d_symTable;
        bool d_success = true;
        // Pass 1
        void generateSymbolTable();
        // Pass 2
        void generateLinkedFile();
        // Gets a symbol, which is a string followed by a integer
        // Can also be used for address
        std::pair<std::string, int> getSymbol(Parser& parser) const;
        // Get the next non-empty token
        std::string parseIgnoreEmpty(Parser& parser) const;
        // Get the size of the define section
        int getDefineSize(Parser& parse);
        // Helper function for 0 padding the total width of a numerical output to totalWidth
        std::string padZeroOutput(int output, int totalWidth);
        void printError(ErrorCode error, int lineNumber, int offset);
    };
} // close namespace OperatingSystems
} // close namespace NYu