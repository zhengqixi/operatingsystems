#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
#include <fstream>
#include <string>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class RandomGenerator {
    public:
        RandomGenerator(std::ifstream& inputStream);
        // gets a random number using the formula described in lab spec
        int getRandom(int burst);

    private:
        int d_currOffset = 0;
        std::vector<int> d_randomBuff;
    };
}
}
#endif