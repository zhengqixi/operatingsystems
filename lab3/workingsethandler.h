#ifndef WORKING_SET_HANDLER_H
#define WORKING_SET_HANDLER_H
#include "pagehandler.h"
#include "process.h"
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class WorkingSetHandler : public PageHandler {
    public:
        WorkingSetHandler(int numFrames, int tau, bool verbose, std::ostream& output);
        void freeFrame(unsigned int frameIndex);

    private:
        std::vector<long long> d_lastTime;
        unsigned long long d_timeDiff;
        std::ostream& d_output;
        bool d_verbose;
        virtual unsigned int fetchFromFree(unsigned long long currentInst, std::vector<Process>& processList);
        unsigned int selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList);
    };
}
}
#endif