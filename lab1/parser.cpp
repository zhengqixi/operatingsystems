#include "parser.h"
#include <cstring>
#include <string>
namespace NYU {
namespace OperatingSystems {
    Parser::Parser(std::string fileName, std::string delim)
        : d_file(fileName)
        , d_delim(delim)
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
            if (d_delim.find(itr) != std::string::npos) {
                itr = '\0';
                betweenTerm = true;
            } else if (betweenTerm) {
                ++wordsCount;
                betweenTerm = false;
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
        d_lastLine = d_currLine;
        d_lastColumn = d_currColumn;
        if (d_wordsLeftOnLine == 0) {
            loadLine();
            d_currColumn = 0;
            d_currColumnEnd = 0;
        }
        if (!continueParsing()) {
            d_currentToken.clear();
            return;
        }
        --d_wordsLeftOnLine;
        d_currColumn = d_currLineString.find_first_not_of('\0', d_currColumnEnd);
        d_currColumnEnd = d_currLineString.find_first_of('\0', d_currColumn);
        d_currentToken = d_currLineString.substr(d_currColumn, d_currColumnEnd - d_currColumn);
    }
    const std::string& Parser::getToken() const
    {
        return d_currentToken;
    }
    const std::string& Parser::parseToken() const
    {
        return d_currentToken;
    }
    int Parser::currTokenColumn() const
    {
        return d_currColumn + 1;
    }
    int Parser::currTokenLine() const
    {
        return d_currLine;
    }
    int Parser::lastTokenColumn() const
    {
        return d_lastColumn + 1;
    }
    int Parser::lastTokenLine() const
    {
        return d_lastLine;
    }
}
}