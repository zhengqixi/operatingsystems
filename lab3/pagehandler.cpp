#include "pagehandler.h"
#include "constants.h"
#include "frame.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    PageHandler::PageHandler()
        : d_globalFrame{ maxFrames }
    {
    }
    Frame& PageHandler::getFrame(unsigned int frameIndex)
    {
        return d_globalFrame[frameIndex];
    }
    Frame& PageHandler::selectFrame()
    {
        if (!d_freeFrames.empty()) {
            unsigned int next = d_freeFrames.front();
            d_freeFrames.pop();
            return d_globalFrame[next];
        }
        return selectVictimFrame();
    }
}
}