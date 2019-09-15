#include "parser.h"
#include <string>
namespace NYU {
namespace OperatingSystems {
    Parser::Parser(std::string fileName)
        : d_file(fileName)
    {
        loadLineParser();
    }
    void Parser::loadLineParser()
    {
        std::string newLine;
        std::getline(d_file, newLine);
        d_lineParser.clear();
        d_lineParser << newLine;
        ++d_currLine;
    }
    std::string Parser::parseToken()
    {
        std::string token;
        d_lineParser >> token;
        // If line parse is empty, get new line, and reload string tokenizer
        if (d_lineParser.eof() && !d_file.eof()) {
            loadLineParser();
        }
        return token;
    }
    bool Parser::continueParsing() const
    {
        return !d_file.eof() && !d_lineParser.eof();
    }
    int Parser::currLine() const
    {
        return d_currLine;
    }
    int Parser::currColumn() const
    {
        return -1;
    }
}
}