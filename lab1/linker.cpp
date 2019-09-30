#include "linker.h"
#include "parser.h"
#include "symboltable.h"
#include <cctype>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    const std::string Linker::d_delimiters = "\n\t ";
    Linker::Linker(std::string fileName, std::ostream& output)
        : d_fileName(fileName)
        , d_output(output)
    {
    }
    void Linker::link()
    {
        generateSymbolTable();
        if (!d_success) {
            return;
        }
        generateLinkedFile();
    }

    void Linker::generateSymbolTable()
    {
        int absoluteAddress = 0;
        int moduleNumber = 1;
        Parser parse(d_fileName, d_delimiters);
        while (parse.continueParsing()) {
            int defineSize = getNumber(parse);
            if (!d_success) {
                return;
            }
            if (defineSize > d_maxDefSize) {
                printError(TOO_MANY_DEF_IN_MODULE, parse.lastTokenLine(), parse.lastTokenColumn());
                return;
            }
            // We store them here to check for rule 5, which we need the instruction size first to check
            std::vector<std::pair<std::string, int>> symbols;
            for (int i = 0; i != defineSize; ++i) {
                auto symbol = getSymbol(parse);
                if (!d_success) {
                    return;
                }
                symbols.push_back(symbol);
            }
            int useListSize = getNumber(parse);
            if (!d_success) {
                return;
            }
            if (useListSize > d_maxUseSize) {
                printError(TOO_MANY_USE_IN_MODULE, parse.lastTokenLine(), parse.lastTokenColumn());
                return;
            }
            for (int i = 0; i != useListSize; ++i) {
                auto token = parse.getToken();
                if (token.empty()) {
                    printError(SYM_EXPECTED, parse.eofLine(), parse.eofColumn());
                    return;
                }
                if (!isValidSymbol(token)) {
                    printError(SYM_EXPECTED, parse.currTokenLine(), parse.currTokenColumn());
                    return;
                }
                parse.nextToken();
            }
            int instructionSize = getNumber(parse);
            if (!d_success) {
                return;
            }
            if ((absoluteAddress + instructionSize) > d_maxIntrSize) {
                printError(TOO_MANY_INSTR, parse.lastTokenLine(), parse.lastTokenColumn());
                return;
            }
            // No we can check for rule 5 and add to the symbol table
            for (auto& symbol : symbols) {
                if (symbol.second > instructionSize) {
                    d_output << "Warning: Module " << moduleNumber << ": ";
                    d_output << symbol.first << " too big " << symbol.second;
                    d_output << " (max=" << (instructionSize - 1) << ") assume zero relative\n";
                    symbol.second = 0;
                }
                d_symTable.addSymbol(symbol.first, symbol.second + absoluteAddress, moduleNumber);
            }
            for (int i = 0; i != instructionSize; ++i) {
                auto instr = getInstr(parse);
                if (!d_success) {
                    return;
                }
            }
            absoluteAddress += instructionSize;
            ++moduleNumber;
        }
    }
    bool Linker::isValidOpcode(int& opcode, int& operand, std::string& errMsg) const
    {
        if (opcode > 9 || opcode < 0) {
            opcode = 9;
            operand = 999;
            errMsg = "Error: Illegal opcode; treated as 9999";
            return false;
        }
        return true;
    }
    void Linker::generateLinkedFile()
    {
        Parser parse(d_fileName, d_delimiters);
        d_output << d_symTable;
        d_output << "\nMemory Map\n";
        int absoluteAddress = 0;
        int currentAddress = 0;
        int moduleNumber = 1;
        std::unordered_set<std::string> masterActuallyUsedList;
        while (parse.continueParsing()) {
            int defineSize = getNumber(parse);
            if (defineSize == -1) {
                break;
            }
            for (int i = 0; i != defineSize; ++i) {
                getSymbol(parse);
            }
            std::vector<std::string> useList;
            int useListSize = getNumber(parse);
            for (int i = 0; i != useListSize; ++i) {
                auto useSymbol = parse.getToken();
                useList.push_back(useSymbol);
                parse.nextToken();
            }
            // Get ready for non-mentioned rule differntiation
            std::unordered_set<std::string> actuallyUsedList;
            int instructionSize = getNumber(parse);
            for (int i = 0; i != instructionSize; ++i) {
                std::string instrError;
                auto instruction = getInstr(parse);
                int opcode = instruction.second / 1000;
                int operand = instruction.second % 1000;
                char addressMode = instruction.first;
                switch (addressMode) {
                case 'R':
                    if (!isValidOpcode(opcode, operand, instrError)) {
                        break;
                    }
                    if (operand > instructionSize) {
                        operand = absoluteAddress;
                        instrError = "Error: Relative address exceeds module size; zero used";
                    } else {
                        operand += absoluteAddress;
                    }
                    break;
                case 'E': {
                    if (!isValidOpcode(opcode, operand, instrError)) {
                        break;
                    }
                    int useListIndex = useList.size() - 1;
                    if (operand > useListIndex) {
                        instrError = "Error: External address exceeds length of uselist; treated as immediate";
                    } else {
                        auto useSymbol = useList[operand];
                        operand = d_symTable.getSymbol(useSymbol);
                        if (operand < 0) {
                            std::stringstream err;
                            err << "Error: " << useSymbol << " is not defined; zero used";
                            instrError = err.str();
                            operand = 0;
                        }
                        actuallyUsedList.insert(useSymbol);
                    }
                } break;
                case 'I':
                    // We DO NOT actually check the opcode here because this immediate value warning takes priority
                    // This is not actually documented anywhere it just is there if you check the reference implementation
                    // UGHHHHHHHHHHHH WHAT THE FUCK
                    if (instruction.second > 9999) {
                        opcode = 9;
                        operand = 999;
                        instrError = "Error: Illegal immediate value; treated as 9999";
                    }
                    // Do nothing
                    break;
                case 'A':
                    if (!isValidOpcode(opcode, operand, instrError)) {
                        break;
                    }
                    if (operand > d_maxIntrSize) {
                        operand = 0;
                        instrError = "Error: Absolute address exceeds machine size; zero used";
                    }
                    break;
                }
                d_output << padZeroOutput(currentAddress, 3);
                d_output << ": ";
                int newInstructionCode = opcode * 1000 + operand;
                d_output << padZeroOutput(newInstructionCode, 4);
                if (!instrError.empty()) {
                    d_output << " " << instrError;
                }
                d_output << '\n';
                ++currentAddress;
                masterActuallyUsedList.insert(actuallyUsedList.begin(), actuallyUsedList.end());
            }
            for (const auto& used : useList) {
                if (actuallyUsedList.find(used) == actuallyUsedList.end()) {
                    d_output << "Warning: Module " << moduleNumber << ": ";
                    d_output << used << " appeared in the uselist but was not actually used\n";
                }
            }
            absoluteAddress += instructionSize;
            ++moduleNumber;
        }
        for (const auto& sym : d_symTable.allSymbols()) {
            if (masterActuallyUsedList.find(sym.symbol) == masterActuallyUsedList.end()) {
                d_output << "Warning: Module " << sym.moduleNumber << ": " << sym.symbol;
                d_output << " was defined but never used\n";
            }
        }
    }
    std::pair<std::string, int> Linker::getSymbol(Parser& parser)
    {
        auto token = parser.getToken();
        if (token.empty()) {
            printError(SYM_EXPECTED, parser.eofLine(), parser.eofColumn());
            return std::pair<std::string, int>("", -1);
        }
        if (!isValidSymbol(token)) {
            printError(SYM_EXPECTED, parser.currTokenLine(), parser.currTokenColumn());
            return std::pair<std::string, int>("", -1);
        }
        parser.nextToken();
        int num = getNumber(parser);
        if (!d_success) {
            return std::pair<std::string, int>("", -1);
        }
        return std::pair<std::string, int>(token, num);
    }
    std::pair<char, int> Linker::getInstr(Parser& parser)
    {
        auto token = parser.getToken();
        if (token.empty()) {
            printError(ADDR_EXPECTED, parser.eofLine(), parser.eofColumn());
            return std::pair<char, int>('\0', -1);
        }
        if (token.length() > 1) {
            printError(ADDR_EXPECTED, parser.currTokenLine(), parser.currTokenColumn());
            return std::pair<char, int>('\0', -1);
        }
        char instr = token[0];
        switch (instr) {
        case 'R':
        case 'E':
        case 'I':
        case 'A':
            // If one of these tokens, then not a parse error
            break;
        default:
            printError(ADDR_EXPECTED, parser.currTokenLine(), parser.currTokenColumn());
            return std::pair<char, int>('\0', -1);
        }
        parser.nextToken();
        int num = getNumber(parser);
        if (!d_success) {
            return std::pair<char, int>('\0', -1);
        }
        return std::pair<char, int>(instr, num);
    }
    std::string Linker::padZeroOutput(int output, int totalWidth)
    {
        std::stringstream formatter;
        formatter << std::setw(totalWidth) << std::setfill('0');
        formatter << output;
        return formatter.str();
    }
    void Linker::printError(ErrorCode error, int lineNumber, int offset)
    {
        d_success = false;
        const static std::string errors[] = {
            "NUM_EXPECTED",
            "SYM_EXPECTED",
            "ADDR_EXPECTED",
            "SYM_TOO_LONG",
            "TOO_MANY_DEF_IN_MODULE",
            "TOO_MANY_USE_IN_MODULE",
            "TOO_MANY_INSTR"
        };
        d_output << "Parse Error line " << lineNumber;
        d_output << " offset " << offset;
        d_output << ": " << errors[error] << '\n';
    }
    int Linker::getNumber(Parser& parser)
    {
        int num = -1;
        auto token = parser.getToken();
        if (token.empty()) {
            printError(NUM_EXPECTED, parser.eofLine(), parser.eofColumn());
            return num;
        }
        if (!isNumber(token)) {
            printError(NUM_EXPECTED, parser.currTokenLine(), parser.currTokenColumn());
            return num;
        }
        num = std::stoi(token);
        parser.nextToken();
        return num;
    }
    bool Linker::isValidSymbol(const std::string& symbol) const
    {
        if (symbol.length() > d_maxSymLength) {
            return false;
        }
        if (!std::isalpha(symbol[0])) {
            return false;
        }
        for (char s : symbol) {
            if (!std::isalnum(s)) {
                return false;
            }
        }
        return true;
    }
    bool Linker::isNumber(const std::string& symbol) const
    {
        for (char s : symbol) {
            if (!std::isdigit(s)) {
                return false;
            }
        }
        return true;
    }
} // close namespace OperatingSystems
} // close namespace NYU
