#include <fstream>
#include <string>
#include <vector>
#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H
namespace NYU {
namespace OperatingSystems {
    class RandomGenerator {
    public:
        RandomGenerator(std::ifstream& inputStream);
        int getRandom(int burst);

    private:
        int d_currOffset = 0;
        std::vector<int> d_randomBuff;
    };
}
}
#endif