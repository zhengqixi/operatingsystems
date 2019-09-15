#include "linker.h"
#include <iostream>

int main(int argc, char* argv[])
{
    using namespace std;
    using namespace NYU::OperatingSystems;
    if (argc != 2) {
        cout << "Must include exactly one file to link\n";
        return -1;
    }
    Linker linkFiles(argv[1], cout);
    linkFiles.link();
    return 0;
}