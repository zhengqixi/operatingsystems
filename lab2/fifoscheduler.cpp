#include "fifoscheduler.h"
namespace NYU {
namespace OperatingSystems {
    FifoScheduler::FifoScheduler()
    {
    }
    FifoScheduler::FifoScheduler(int quantum)
    {
        d_quantum = quantum;
    }
    Process* FifoScheduler::getProcess()
    {
        if (d_queue.empty()) {
            return nullptr;
        }
        auto newProcess = d_queue.front();
        d_queue.pop();
        return newProcess;
    }
    void FifoScheduler::addProcess(Process* toSchedule)
    {
        d_queue.push(toSchedule);
    }
}
}