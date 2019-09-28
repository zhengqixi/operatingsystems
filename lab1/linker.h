#include "parser.h"
#include "symboltable.h"
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
        const static int d_maxSymLength = 16;
        const static std::string d_delimiters;
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
        std::pair<std::string, int> getSymbol(Parser& parser);
        // Gets an address
        std::pair<char, int> getInstr(Parser& parser);
        // Helper function for 0 padding the total width of a numerical output to totalWidth
        std::string padZeroOutput(int output, int totalWidth);
        void printError(ErrorCode error, int lineNumber, int offset);
        // Will set d_success to false if not a number
        // The return value will be -1
        // If success, then the parser will be set to get the next token
        int getNumber(Parser& parser);
        // Re
        bool isValidOpcode(int& opcode, int& operand, std::string& errMsg) const;
        bool isValidSymbol(const std::string& symbol) const;
    };
} // close namespace OperatingSystems
} // close namespace NYu