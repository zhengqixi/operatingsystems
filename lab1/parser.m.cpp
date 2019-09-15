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
    return 0;
}