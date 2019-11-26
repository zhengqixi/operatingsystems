#include "clockhandler.h"
#include "pagehandler.h"
#include "process.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    ClockHandler::ClockHandler(int numFrames)
        : PageHandler(numFrames)
    {
    }

    unsigned int ClockHandler::selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList)
    {
        int offset = d_offset;
        while (1) {
            int process = d_globalFrame[offset].mappedProcess();
            int page = d_globalFrame[offset].mappedPage();
            if (processList[process][page].referenced()) {
                processList[process][page].referenced(false);
            } else {
                d_offset = (offset + 1) % d_globalFrame.size();
                return static_cast<unsigned int>(offset);
            }
            offset = (offset + 1) % d_globalFrame.size();
        }
    }
}
}