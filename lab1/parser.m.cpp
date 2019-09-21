#include "parser.h"
#include <iostream>
int main(int argc, char* argv[])
{
    using namespace std;
    using namespace NYU::OperatingSystems;
    if (argc != 2) {
        cout << "Must include exactly one file to parse\n";
        return -1;
    }
    Parser fileParser(argv[1]);
    while (fileParser.continueParsing()) {

        int currLine = fileParser.currTokenLine();
        int currColumn = fileParser.currTokenColumn();
        auto token = fileParser.getToken();
        cout << "Token: " << token;
        cout << " on line: " << currLine;
        cout << " and column: " << currColumn;
        cout << '\n';
        fileParser.nextToken();
    }
    return 0;
}