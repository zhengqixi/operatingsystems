#include "linker.h"
#include "SymbolTable.h"
#include "parser.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
namespace NYU {
namespace OperatingSystems {
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
        Parser parse(d_fileName);
        while (parse.continueParsing()) {
            int defineSize = getDefineSize(parse);
            if (defineSize == -1) {
                break;
            }
            if (defineSize > d_maxDefSize) {
                printError(TOO_MANY_DEF_IN_MODULE, 0, 0);
                return;
            }
            for (int i = 0; i != defineSize; ++i) {
                auto symbol = getSymbol(parse);
                int absoluteAddr = symbol.second + absoluteAddress;
                d_symTable.addSymbol(symbol.first, absoluteAddr);
            }
            int useListSize = std::stoi(parseIgnoreEmpty(parse));
            if (useListSize > d_maxUseSize) {
                printError(TOO_MANY_USE_IN_MODULE, 0, 0);
                return;
            }
            for (int i = 0; i != useListSize; ++i) {
                // Just parse it for now
                parseIgnoreEmpty(parse);
            }
            int instructionSize = std::stoi(parseIgnoreEmpty(parse));
            absoluteAddress += instructionSize;
            if (absoluteAddress > d_maxIntrSize) {
                printError(TOO_MANY_INSTR, 0, 0);
                return;
            }
            for (int i = 0; i != instructionSize; ++i) {
                // Just parse it for now
                getSymbol(parse);
            }
        }
    }
    int Linker::getDefineSize(Parser& parse)
    {
        auto defineSizeStr = parseIgnoreEmpty(parse);
        if (defineSizeStr.empty()) {
            return -1;
        }
        return std::stoi(defineSizeStr);
    }

    std::pair<std::string, int> Linker::getSymbol(Parser& parser) const
    {
        std::string symbol = parseIgnoreEmpty(parser);
        int relAddr = std::stoi(parseIgnoreEmpty(parser));
        return std::pair<std::string, int>(symbol, relAddr);
    }

    std::string Linker::parseIgnoreEmpty(Parser& parser) const
    {
        while (parser.continueParsing()) {
            auto token = parser.parseToken();
            if (!token.empty()) {
                return token;
            }
        }
        return "";
    }

    void Linker::generateLinkedFile()
    {
        Parser parse(d_fileName);
        d_output << d_symTable;
        d_output << "\nMemory Map\n";
        int absoluteAddress = 0;
        int currentAddress = 0;
        int moduleNumber = 1;
        while (parse.continueParsing()) {
            int defineSize = getDefineSize(parse);
            if (defineSize == -1) {
                break;
            }
            for (int i = 0; i != defineSize; ++i) {
                getSymbol(parse);
            }
            std::vector<std::string> useList;
            int useListSize = std::stoi(parseIgnoreEmpty(parse));
            for (int i = 0; i != useListSize; ++i) {
                auto useSymbol = parseIgnoreEmpty(parse);
                useList.push_back(useSymbol);
            }
            int instructionSize = std::stoi(parseIgnoreEmpty(parse));
            for (int i = 0; i != instructionSize; ++i) {
                auto instruction = getSymbol(parse);
                int opcode = instruction.second / 1000;
                int operand = instruction.second % 1000;
                char addressMode = instruction.first.at(0);
                switch (addressMode) {
                case 'R':
                    operand += absoluteAddress;
                    break;
                case 'E': {
                    auto useSymbol = useList[operand];
                    operand = d_symTable.getSymbol(useSymbol);
                } break;
                case 'I':
                    // Do nothing
                    break;
                case 'A':
                    // Do nothing
                    break;
                }
                d_output << padZeroOutput(currentAddress, 3);
                d_output << ": ";
                int newInstructionCode = opcode * 1000 + operand;
                d_output << padZeroOutput(newInstructionCode, 4) << '\n';
                ++currentAddress;
            }
            absoluteAddress += instructionSize;
            ++moduleNumber;
        }
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
} // close namespace OperatingSystems
} // close namespace NYU