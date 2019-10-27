#include "fifoscheduler.h"
namespace NYU {
namespace OperatingSystems {
    Process* FifoScheduler::getProcess()
    {
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