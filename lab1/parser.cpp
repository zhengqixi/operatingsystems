#include "parser.h"
#include <cstring>
#include <string>
namespace NYU {
namespace OperatingSystems {
    Parser::Parser(std::string fileName)
        : d_file(fileName)
    {
        nextToken();
    }
    void Parser::loadLine()
    {
        while (!d_file.eof()) {
            ++d_currLine;
            std::string line;
            std::getline(d_file, line);
            int wordsCount = manualTokenize(line);
            if (wordsCount == 0) {
                continue;
            }
            d_currLineString.swap(line);
            d_wordsLeftOnLine = wordsCount;
            return;
        }
    }
    int Parser::manualTokenize(std::string& line) const
    {
        int wordsCount = 0;
        bool betweenTerm = true;
        for (auto& itr : line) {
            switch (itr) {
            case ' ':
                itr = '\0';
                betweenTerm = true;
                break;
            case '\t':
                itr = '\0';
                betweenTerm = true;
                break;
            case '\n':
                // Shouldn't really encouter this since we getline but just in case...
                itr = '\0';
                betweenTerm = true;
                break;
            default:
                if (betweenTerm) {
                    ++wordsCount;
                    betweenTerm = false;
                }
                break;
            }
        }
        return wordsCount;
    }
    bool Parser::continueParsing() const
    {
        if (d_file.eof() && d_wordsLeftOnLine == 0) {
            return false;
        }
        return true;
    }
    void Parser::nextToken()
    {
        if (d_wordsLeftOnLine == 0) {
            loadLine();
            d_currColumn = 0;
            d_currColumnEnd = 0;
        }
        if (!continueParsing()) {
            return;
        }
        --d_wordsLeftOnLine;
        d_currColumn = d_currLineString.find_first_not_of('\0', d_currColumnEnd);
        d_currColumnEnd = d_currLineString.find_first_of('\0', d_currColumn);
        d_currentToken = d_currLineString.substr(d_currColumn, d_currColumnEnd - d_currColumn);
    }
    std::string Parser::getToken() const
    {
        return d_currentToken;
    }
    std::string Parser::parseToken() const
    {
        return d_currentToken;
    }
    int Parser::currTokenColumn() const
    {
        return d_currColumn;
    }
    int Parser::currTokenLine() const
    {
        return d_currLine;
    }
}
}