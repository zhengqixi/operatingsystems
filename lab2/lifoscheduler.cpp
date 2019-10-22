#include "lifoscheduler.h"
namespace NYU {
namespace OperatingSystems {
    std::shared_ptr<Process> LifoScheduler::getProcess()
    {
        auto newProcess = d_stack.top();
        d_stack.pop();
        return newProcess;
    }
    void LifoScheduler::addProcess(std::shared_ptr<Process> toSchedule)
    {
        d_stack.push(toSchedule);
    }
}
}