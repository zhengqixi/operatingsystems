#include "nruhandler.h"
#include "pagehandler.h"
#include "process.h"
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    NRUHandler::NRUHandler(int numFrames, int scanFreq, bool verbose, std::ostream& output)
        : PageHandler{ numFrames }
        , d_freq{ static_cast<unsigned int>(scanFreq) }
        , d_verbose{ verbose }
        , d_output{ output }
    {
    }

    int NRUHandler::getClass(const Page& page) const
    {
        return 2 * static_cast<int>(page.referenced())
            + static_cast<int>(page.modified());
    }
    unsigned int NRUHandler::selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList)
    {
        // Since there are only 4, this will automatically
        // be set on the first check to a valid value
        int lowestClass = 4;
        unsigned int selectedFrame = 0;
        bool reset = (currentInst - d_lastScan) >= d_freq;
        if (reset) {
            d_lastScan = currentInst;
        }
        if (d_verbose) {
            d_output << "ASELECT: hand= " << d_offset << ' ' << reset;
            d_output << " | ";
        }
        int counter = 0;
        int frameIndex = d_offset;
        while (counter != d_globalFrame.size()) {
            ++counter;
            int process = d_globalFrame[frameIndex].mappedProcess();
            int page = d_globalFrame[frameIndex].mappedPage();
            int pageClass = getClass(processList[process][page]);
            if (pageClass < lowestClass) {
                selectedFrame = static_cast<unsigned int>(frameIndex);
                d_offset = (frameIndex + 1) % d_globalFrame.size();
                lowestClass = pageClass;
                if (lowestClass == 0 && !reset) {
                    break;
                }
            }
            if (reset) {
                processList[process][page].referenced(false);
            }
            frameIndex = (frameIndex + 1) % d_globalFrame.size();
        }
        if (d_verbose) {
            d_output << lowestClass << ' ';
            d_output << selectedFrame << ' ';
            d_output << counter << '\n';
        }
        return selectedFrame;
    }
}
}