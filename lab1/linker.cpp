#include "linker.h"
#include "SymbolTable.h"
#include "parser.h"
#include <iostream>
namespace NYU {
namespace OperatingSystems {
    Linker::Linker(std::ifstream& input, std::ostream& output)
        : d_input(input)
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
        Parser parse(d_input);
        while (parse.continueParsing()) {
        }
    }
    void Linker::generateLinkedFile(const SymbolTable& symtable)
    {
        Parser parse(d_input);
        while (parse.continueParsing()) {
        }
    }
}
}