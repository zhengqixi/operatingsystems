#include "prioscheduler.h"
#include "process.h"
#include <strings.h>
#include <utility>
#include <vector>
#define BIT_SET(a, b) ((a) |= (1UL << (b)))
#define BIT_CLEAR(a, b) ((a) &= ~(1UL << (b)))
#define vectorSize std::vector<NYU::OperatingSystems::Process*>::size_type
namespace NYU {
namespace OperatingSystems {
    PRIOScheduler::DecayQueue::DecayQueue(int maxPriority)
        : d_decayQueue{ static_cast<vectorSize>(maxPriority) }
    {
    }
    Process* PRIOScheduler::DecayQueue::getProcess()
    {
        if (d_processCount == 0) {
            return nullptr;
        }
        --d_processCount;
        Process* process = nullptr;
        for (auto i = d_decayQueue.rbegin(); i != d_decayQueue.rend(); ++i) {
            if (!i->empty()) {
                process = i->front();
                i->pop();
                break;
            }
        }
        /*
        Bit manipulation too hard :(
        // Get highest priority bit. Subtract one to account for vector 0 indexing
        // This should not be -1. If it is, we have an issue with accounting in process count
        int highestPriority = ffs(d_firstSetBit) - 1;
        // pop it from the vector
        auto process = d_decayQueue.at(highestPriority).front();
        d_decayQueue.at(highestPriority).pop();
        // if queue for priority is now empty, unset the bit
        if (d_decayQueue.at(highestPriority).empty()) {
            BIT_CLEAR(d_firstSetBit, highestPriority);
        }
        */
        return process;
    }
    void PRIOScheduler::DecayQueue::addProcess(Process* process)
    {
        ++d_processCount;
        int priority = process->dynamicPriority();
        d_decayQueue.at(priority).push(process);
        // BIT_SET(d_firstSetBit, priority);
    }
    bool PRIOScheduler::DecayQueue::empty() const
    {
        return d_processCount == 0;
    }
    PRIOScheduler::PRIOScheduler(int quantum, int maxPriority)
        : d_active{ maxPriority }
        , d_expired{ maxPriority }
    {
        d_quantum = quantum;
        d_priorityMax = maxPriority;
    }
    Process* PRIOScheduler::getProcess()
    {
        if (d_active.empty() && d_expired.empty()) {
            return nullptr;
        }
        if (d_active.empty()) {
            std::swap(d_active, d_expired);
        }
        auto nextProcess = d_active.getProcess();
        return nextProcess;
    }
    void PRIOScheduler::addProcess(Process* toSchedule)
    {
        if (toSchedule->dynamicPriority() == -1) {
            toSchedule->resetDynamicPriority();
            d_expired.addProcess(toSchedule);
        } else {
            d_active.addProcess(toSchedule);
        }
    }
}
}