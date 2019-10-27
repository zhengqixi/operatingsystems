#include "fifoscheduler.h"
namespace NYU {
namespace OperatingSystems {
    FIFOScheduler::FIFOScheduler()
    {
    }
    FIFOScheduler::FIFOScheduler(int quantum)
    {
        d_quantum = quantum;
    }
    Process* FIFOScheduler::getProcess()
    {
        if (d_queue.empty()) {
            return nullptr;
        }
        auto newProcess = d_queue.front();
        d_queue.pop();
        return newProcess;
    }
    void FIFOScheduler::addProcess(Process* toSchedule)
    {
        d_queue.push(toSchedule);
    }
}
}