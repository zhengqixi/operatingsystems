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
        auto symTable = generateSymbolTable();
        generateLinkedFile(symTable);
    }

    SymbolTable Linker::generateSymbolTable()
    {
        SymbolTable table;
        int absoluteAddress = 0;
        Parser parse(d_fileName);
        while (parse.continueParsing()) {
            int defineSize = std::stoi(parseIgnoreEmpty(parse));
            for (int i = 0; i != defineSize; ++i) {
                auto symbol = getSymbol(parse);
                int absoluteAddr = symbol.second + absoluteAddress;
                table.addSymbol(symbol.first, absoluteAddr);
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
        return table;
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

    void Linker::generateLinkedFile(const SymbolTable& symtable)
    {
        Parser parse(d_fileName);
        d_output << symtable;
        d_output << "\nMemory Map\n";
        int absoluteAddress = 0;
        int currentAddress = 0;
        int moduleNumber = 1;
        while (parse.continueParsing()) {
            int defineSize = std::stoi(parseIgnoreEmpty(parse));
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
                    operand = symtable.getSymbol(useSymbol);
                } break;
                case 'I':
                    // Do nothing
                    break;
                case 'A':
                    // Do nothing
                    break;
                }
                std::stringstream addressFormatter;
                addressFormatter << std::setw(4) << std::setfill('0');
                addressFormatter << currentAddress;
                std::string addressString;
                addressFormatter >> addressString;
                d_output << addressString << ": ";
                int newInstructionCode = opcode * 1000 + operand;
                d_output << newInstructionCode << '\n';
                ++currentAddress;
            }
            absoluteAddress += instructionSize;
            ++moduleNumber;
        }
    }
}
}