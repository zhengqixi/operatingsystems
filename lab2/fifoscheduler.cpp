#include "fifoscheduler.h"
namespace NYU {
namespace OperatingSystems {
    std::shared_ptr<Process> FifoScheduler::getProcess()
    {
        auto newProcess = d_queue.front();
        d_queue.pop();
        return newProcess;
    }
    void FifoScheduler::addProcess(std::shared_ptr<Process> toSchedule)
    {
        d_queue.push(toSchedule);
    }
}
}