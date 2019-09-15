#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#ifndef PARSER_H
#define PARSER_H
namespace NYU {
namespace OperatingSystems {
    class Parser {
    public:
        Parser(std::ifstream& parseFile);
        ~Parser();
        std::string parseToken();
        bool continueParsing() const;
        int currLine() const;
        int currColumn() const;
        std::pair<std::string, int> parseSymbol();
        std::pair<char, int> parseInstruction();

    private:
        void loadLineParser();
        std::ifstream& d_file;
        std::stringstream d_lineParser;
        int d_currLine = 0;
    };
} // close namespace operating systems

} // close namespace nyu

#endif