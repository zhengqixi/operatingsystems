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
        bool success = generateSymbolTable();
        if (!success) {
            return;
        }
        generateLinkedFile();
    }

    bool Linker::generateSymbolTable()
    {
        int absoluteAddress = 0;
        Parser parse(d_fileName);
        while (parse.continueParsing()) {
            int defineSize = getDefineSize(parse);
            if (defineSize == -1) {
                break;
            }
            for (int i = 0; i != defineSize; ++i) {
                auto symbol = getSymbol(parse);
                int absoluteAddr = symbol.second + absoluteAddress;
                d_symTable.addSymbol(symbol.first, absoluteAddr);
            }
            int useListSize = std::stoi(parseIgnoreEmpty(parse));
            for (int i = 0; i != useListSize; ++i) {
                // Just parse it for now
                parseIgnoreEmpty(parse);
            }
            int instructionSize = std::stoi(parseIgnoreEmpty(parse));
            for (int i = 0; i != instructionSize; ++i) {
                // Just parse it for now
                getSymbol(parse);
            }
            absoluteAddress += instructionSize;
        }
        return true;
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
} // close namespace OperatingSystems
} // close namespace NYU