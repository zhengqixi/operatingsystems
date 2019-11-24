#include "fifohandler.h"
#include "pagehandler.h"
namespace NYU {
namespace OperatingSystems {
    FifoHandler::FifoHandler(int numFrames)
        : PageHandler(numFrames)
    {
    }

    unsigned int FifoHandler::selectVictimFrame()
    {
        auto index = d_currentIndex;
        d_currentIndex = (++d_currentIndex) % d_globalFrame.size();
        return index;
    }
}
}