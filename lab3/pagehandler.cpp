#include "pagehandler.h"
#include "frame.h"
#include <vector>
namespace NYU {
namespace OperatingSystems {
    PageHandler::PageHandler(int numFrames)
        : d_globalFrame{ static_cast<unsigned long int>(numFrames) }
    {
        for (int i = 0; i < numFrames; ++i) {
            d_freeFrames.push(static_cast<unsigned int>(i));
        }
    }
    Frame& PageHandler::getFrame(unsigned int frameIndex)
    {
        return d_globalFrame[frameIndex];
    }
    unsigned int PageHandler::selectFrame()
    {
        if (!d_freeFrames.empty()) {
            unsigned int next = d_freeFrames.front();
            d_freeFrames.pop();
            return next;
        }
        return selectVictimFrame();
    }
    void PageHandler::freeFrame(unsigned int frameIndex)
    {
        d_globalFrame[frameIndex].unmap();
        d_freeFrames.push(frameIndex);
    }
    const std::vector<Frame>& PageHandler::globalFrames() const
    {
        return d_globalFrame;
    }
}
}