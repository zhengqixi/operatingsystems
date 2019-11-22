#ifndef PAGE_HANDLER_H
#define PAGE_HANDLER_H
#include "frame.h"
#include <queue>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class PageHandler {
    public:
        PageHandler();
        // Returns a frame from the global table by index
        Frame& getFrame(unsigned int frameIndex);
        // Selects a frame from either free or victim for use
        Frame& selectFrame();

    private:
        virtual Frame& selectVictimFrame() = 0;
        std::vector<Frame> d_globalFrame;
        std::queue<unsigned int> d_freeFrames;
    };
}
}
#endif