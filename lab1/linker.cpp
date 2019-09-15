#include "linker.h"
#include "SymbolTable.h"
#include "parser.h"
#include <iostream>
#include <string>
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
        Parser parse(d_fileName);
        while (parse.continueParsing()) {
        }
    }
    void Linker::generateLinkedFile(const SymbolTable& symtable)
    {
        Parser parse(d_fileName);
        while (parse.continueParsing()) {
        }
    }
}
}