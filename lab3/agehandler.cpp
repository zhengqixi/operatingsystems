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
        unsigned int selectedFrame = d_offset;
        unsigned int hand = d_offset;
        unsigned int lowestAge = std::numeric_limits<unsigned int>::max();
        if (d_verbose) {
            unsigned int stoppingPoint = hand == 0 ? d_globalFrame.size() - 1 : hand - 1;
            d_output << "ASELECT " << hand << '-' << stoppingPoint;
            d_output << " | ";
        }
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
            if (d_verbose) {
                d_output << hand << ':';
                d_output << age;
                d_output << ' ';
            }
            hand = (hand + 1) % d_globalFrame.size();

        } while (hand != d_offset);
        d_offset = (selectedFrame + 1) % d_globalFrame.size();
        d_ages[selectedFrame] = 0;
        if (d_verbose) {
            d_output << "| " << selectedFrame << '\n';
        }
        return selectedFrame;
    }
}
}