#ifndef AGE_HANDLER_H
#define AGE_HANDLER_H
#include "pagehandler.h"
#include "process.h"
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class AgeHandler : public PageHandler {
    public:
        AgeHandler(int numFrames, bool verbose, std::ostream& output);
        void freeFrame(unsigned int frameIndex);

    private:
        std::vector<unsigned int> d_ages;
        std::ostream& d_output;
        bool d_verbose;
        unsigned int selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList);
    };
}
}
#endif