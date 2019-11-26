#include "fifohandler.h"
#include "pagehandler.h"
#include "process.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    FifoHandler::FifoHandler(int numFrames)
        : PageHandler(numFrames)
    {
    }

    unsigned int FifoHandler::selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList)
    {
        auto index = d_offset;
        d_offset = (d_offset + 1) % d_globalFrame.size();
        return index;
    }
}
}