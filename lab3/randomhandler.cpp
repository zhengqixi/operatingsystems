#include "randomhandler.h"
#include "process.h"
#include <fstream>
#include <string>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    RandomHandler::RandomHandler(int numFrames, const std::string& randomFile)
        : PageHandler( numFrames )
    {
        std::ifstream inputStream(randomFile);
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

    unsigned int RandomHandler::selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList)
    {
        int random = d_randomBuff[d_offset];
        d_offset = (d_offset + 1) % d_randomBuff.size();
        return random % d_globalFrame.size();
    }
}
}