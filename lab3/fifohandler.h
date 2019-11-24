#ifndef FIFO_HANDLER_H
#define FIFO_HANDLER_H
#include "pagehandler.h"
namespace NYU {
namespace OperatingSystems {
    class FifoHandler : public PageHandler {
    public:
        FifoHandler(int numFrames);

    private:
        unsigned int d_currentIndex = 0;
        unsigned int selectVictimFrame();
    };
}
}
#endif