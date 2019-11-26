#ifndef RANDOM_HANDLER_H
#define RANDOM_HANDLER_H
#include "pagehandler.h"
#include "process.h"
#include <string>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class RandomHandler : public PageHandler {
    public:
        RandomHandler(int numFrames, const std::string& randomFile);

    private:
        std::vector<int> d_randomBuff;
        unsigned int selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList);
    };
}
}
#endif