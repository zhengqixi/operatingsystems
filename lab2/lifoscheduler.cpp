#include "lifoscheduler.h"
namespace NYU {
namespace OperatingSystems {
    Process* LifoScheduler::getProcess()
    {
        auto newProcess = d_stack.top();
        d_stack.pop();
        return newProcess;
    }
    void LifoScheduler::addProcess(Process* toSchedule)
    {
        d_stack.push(toSchedule);
    }
}
}