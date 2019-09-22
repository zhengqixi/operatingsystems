#include <fstream>
#include <sstream>
#include <string>
#ifndef PARSER_H
#define PARSER_H
namespace NYU {
namespace OperatingSystems {
    class Parser {
    public:
        Parser(std::string fileName, std::string delim);
        // Returns a non-empty token, UNLESS the end of the file has been reached
        // Use getToken() instead. Just calls that internally
        const std::string& parseToken() const;
        // get current token. If an empty string is returned, then end of file has been reached
        const std::string& getToken() const;
        // advances the token to the next one in the line
        // If line is empty, then gets the next line and sets appropriately
        void nextToken();
        bool continueParsing() const;
        int currTokenLine() const;
        int currTokenColumn() const;

    private:
        // Gets a new line that has at least one valid symbol in it
        // This will keep incrementing the line count until then
        void loadLine();
        // strtok on string manually
        // No way to use strtok without mixing and matching cstring and c++ string
        // Which is not only ugly, but would require manual dynamic memory allocations
        // Which honestly should be avoided in modern c++ outside of library code
        // Returns the number of words found
        int manualTokenize(std::string& line) const;
        std::ifstream d_file;
        std::string d_currentToken;
        std::string d_currLineString;
        std::string d_delim;
        // number of words left on the line, excluding current token
        int d_wordsLeftOnLine = 0;
        int d_currLine = 0;
        int d_currColumn = 0;
        int d_currColumnEnd = 0;
    };
} // close namespace operating systems
} // close namespace nyu

#endif