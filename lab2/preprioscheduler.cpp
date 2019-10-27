#include "preprioscheduler.h"
namespace NYU {
namespace OperatingSystems {
    PREPRIOScheduler::PREPRIOScheduler(int quantum, int maxPriority)
    {
        d_quantum = quantum;
        d_priorityMax = maxPriority;
    }
    Process* PREPRIOScheduler::getProcess()
    {
        return nullptr;
    }
    void PREPRIOScheduler::addProcess(Process* toSchedule)
    {
    }
}
}