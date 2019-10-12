#include "randomgenerator.h"
#include <iostream>
int main(int argc, char** argv)
{
    using namespace NYU::OperatingSystems;
    if (argc != 2) {
        return -1;
    }
    std::ifstream input(argv[1]);
    RandomGenerator generator(input);
    std::cout << 40000 << '\n';
    for (int i = 0; i < 40000; ++i) {
        std::cout << generator.getRandom(2000000000) - 1 << '\n';
    }
    return 0;
}