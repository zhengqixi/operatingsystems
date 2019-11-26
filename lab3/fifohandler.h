#ifndef FIFO_HANDLER_H
#define FIFO_HANDLER_H
#include "pagehandler.h"
#include "process.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class FifoHandler : public PageHandler {
    public:
        FifoHandler(int numFrames);

    private:
        unsigned int selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList);
    };
}
}
#endif