#include "randomgenerator.h"
#include <fstream>
#include <string>
namespace NYU {
namespace OperatingSystems {
    RandomGenerator::RandomGenerator(std::ifstream& inputStream)
    {
        int totalCount;
        inputStream >> totalCount;
        d_randomBuff.reserve(totalCount);
        // To prevent weird whitespace bugs with using eof()
        while (d_randomBuff.size() != totalCount) {
            int ranNum;
            inputStream >> ranNum;
            d_randomBuff.push_back(ranNum);
        }
    }
    int RandomGenerator::getRandom(int burst)
    {
        int num = 1 + (d_randomBuff[d_currOffset] % burst);
        ++d_currOffset;
        d_currOffset = d_currOffset % d_randomBuff.size();
        return num;
    }
}
}