#ifndef CLOCK_HANDLER_H
#define CLOCK_HANDLER_H
#include "pagehandler.h"
#include "process.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class ClockHandler : public PageHandler {
    public:
        ClockHandler(int numFrames);

    private:
        unsigned int selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList);
    };
}
}
#endif