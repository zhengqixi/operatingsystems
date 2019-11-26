#include "agehandler.h"
#include "pagehandler.h"
#include "process.h"
#include <limits>
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    AgeHandler::AgeHandler(int numFrames, bool verbose, std::ostream& output)
        : PageHandler(numFrames)
        , d_ages(numFrames)
        , d_verbose(verbose)
        , d_output(output)
    {
    }
    void AgeHandler::freeFrame(unsigned int frameIndex)
    {
        PageHandler::freeFrame(frameIndex);
        d_ages[frameIndex] = 0;
    }
    unsigned int AgeHandler::selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList)
    {
        unsigned int hand = d_offset;
        unsigned int selectedFrame = 0;
        unsigned int lowestAge = std::numeric_limits<unsigned int>::max();
        do {
            int process = d_globalFrame[hand].mappedProcess();
            int page = d_globalFrame[hand].mappedPage();
            unsigned int& age = d_ages[hand];
            age = age >> 1;
            if (processList[process][page].referenced()) {
                age = (age | 0x80000000);
                processList[process][page].referenced(false);
            }
            if (age < lowestAge) {
                lowestAge = age;
                selectedFrame = hand;
            }
            hand = (hand + 1) % d_globalFrame.size();

        } while (hand != d_offset);
        d_offset = (selectedFrame + 1) % d_globalFrame.size();
        d_ages[selectedFrame] = 0;
        return selectedFrame;
    }
}
}