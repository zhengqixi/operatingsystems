#ifndef NRU_HANDLER_H
#define NRU_HANDLER_H
#include "page.h"
#include "pagehandler.h"
#include "process.h"
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class NRUHandler : public PageHandler {
    public:
        NRUHandler(int numFrames, int scanFreq, bool verbose, std::ostream& output);

    private:
        int getClass(const Page& page) const;
        unsigned long long d_freq;
        long long d_lastScan = -1;
        bool d_verbose;
        std::ostream& d_output;
        unsigned int selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList);
    };
}
}
#endif