#include "workingsethandler.h"
#include "pagehandler.h"
#include "process.h"
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    WorkingSetHandler::WorkingSetHandler(int numFrames, int tau, bool verbose, std::ostream& output)
        : PageHandler(numFrames)
        , d_lastTime(numFrames, -1)
        , d_timeDiff(tau)
        , d_verbose(verbose)
        , d_output(output)
    {
    }
    void WorkingSetHandler::freeFrame(unsigned int frameIndex)
    {
        PageHandler::freeFrame(frameIndex);
        d_lastTime[frameIndex] = -1;
    }
    unsigned int WorkingSetHandler::fetchFromFree(unsigned long long currentInst, std::vector<Process>& processList)
    {
        unsigned int selectedFrame = PageHandler::fetchFromFree(currentInst, processList);
        d_lastTime[selectedFrame] = currentInst;
        return selectedFrame;
    }

    unsigned int WorkingSetHandler::selectVictimFrame(unsigned long long currentInst, std::vector<Process>& processList)
    {
        unsigned int selectedFrame = d_offset;
        unsigned long long oldestAge = 0;
        unsigned int hand = d_offset;
        unsigned int framesScanned = 0;
        if (d_verbose) {
            unsigned int stoppingPoint = hand == 0 ? d_globalFrame.size() - 1 : hand - 1;
            d_output << "ASELECT " << hand << '-' << stoppingPoint;
            d_output << " | ";
        }
        while (framesScanned != d_globalFrame.size()) {
            ++framesScanned;
            int process = d_globalFrame[hand].mappedProcess();
            int page = d_globalFrame[hand].mappedPage();
            if (d_verbose) {
                d_output << hand << '(' << processList[process][page].referenced();
                d_output << ' ' << process << ':' << page << ' ' << d_lastTime[hand];
                d_output << ") ";
            }
            if (processList[process][page].referenced()) {
                processList[process][page].referenced(false);
                d_lastTime[hand] = static_cast<long long>(currentInst);
            }
            unsigned long long age = currentInst - d_lastTime[hand];
            if (!processList[process][page].referenced() && age >= d_timeDiff) {
                selectedFrame = hand;
                if (d_verbose) {
                    d_output << "STOP(" << framesScanned << ") ";
                }
                break;
            }
            if (age > oldestAge) {
                selectedFrame = hand;
                oldestAge = age;
            }
            hand = (hand + 1) % d_globalFrame.size();
        }
        d_lastTime[selectedFrame] = static_cast<long long>(currentInst);
        d_offset = (selectedFrame + 1) % d_globalFrame.size();
        if (d_verbose) {
            d_output << "| " << selectedFrame << '\n';
        }
        return selectedFrame;
    }
}
}