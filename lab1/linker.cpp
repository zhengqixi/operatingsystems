#include "linker.h"
#include "SymbolTable.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <utility>
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
                parse.parseToken();
            }
            int instructionSize = std::stoi(parseIgnoreEmpty(parse));
            absoluteAddress += instructionSize;
            for (int i = 0; i != instructionSize; ++i) {
                // Just parse it for now
                getSymbol(parse);
            }
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
        while (parse.continueParsing()) {
            parse.parseToken();
        }
    }
}
}