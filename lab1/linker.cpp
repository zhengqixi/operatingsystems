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
        Parser parse(d_fileName, d_delimiters);
        while (parse.continueParsing()) {
            int defineSize = getNumber(parse);
            if (!d_success) {
                break;
            }
            if (defineSize > d_maxDefSize) {
                printError(TOO_MANY_DEF_IN_MODULE, parse.currTokenLine(), parse.currTokenColumn());
                return;
            }
            for (int i = 0; i != defineSize; ++i) {
                auto symbol = getSymbol(parse);
                if (!d_success) {
                    break;
                }
                int absoluteAddr = symbol.second + absoluteAddress;
                d_symTable.addSymbol(symbol.first, absoluteAddr);
            }
            if (d_success) {
                break;
            }
            int useListSize = getNumber(parse);
            if (useListSize > d_maxUseSize) {
                printError(TOO_MANY_USE_IN_MODULE, parse.currTokenLine(), parse.currTokenColumn());
                return;
            }
            for (int i = 0; i != useListSize; ++i) {
                // Just parse it for now
                parse.nextToken();
            }
            if (d_success) {
                break;
            }
            int instructionSize = getNumber(parse);
            absoluteAddress += instructionSize;
            if (absoluteAddress > d_maxIntrSize) {
                printError(TOO_MANY_INSTR, parse.currTokenLine(), parse.currTokenColumn());
                return;
            }
            for (int i = 0; i != instructionSize; ++i) {
                // Just parse it for now
                getSymbol(parse);
                if (d_success) {
                    break;
                }
            }
            if (d_success) {
                break;
            }
        }
    }
    void Linker::generateLinkedFile()
    {
        Parser parse(d_fileName, d_delimiters);
        d_output << d_symTable;
        d_output << "\nMemory Map\n";
        int absoluteAddress = 0;
        int currentAddress = 0;
        int moduleNumber = 1;
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
            int instructionSize = getNumber(parse);
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
    std::pair<std::string, int> Linker::getSymbol(Parser& parser)
    {
        auto token = parser.getToken();
        if (token.empty()) {
            d_success = false;
            printError(SYM_EXPECTED, parser.currTokenLine(), parser.currTokenColumn());
            return std::pair<std::string, int>("", -1);
        }
        if (token.length() > d_maxSymLength) {
            d_success = false;
            printError(SYM_TOO_LONG, parser.currTokenLine(), parser.currTokenColumn());
            return std::pair<std::string, int>("", -1);
        }
        parser.nextToken();
        int num = getNumber(parser);
        if (!d_success) {
            return std::pair<std::string, int>("", -1);
        }
        return std::pair<std::string, int>(token, num);
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
            d_success = false;
            return num;
        }
        try {
            num = std::stoi(token);
            parser.nextToken();
        } catch (const std::invalid_argument& e) {
            d_success = false;
            printError(NUM_EXPECTED, parser.currTokenLine(), parser.currTokenColumn());
        }
        return num;
    }
} // close namespace OperatingSystems
} // close namespace NYU