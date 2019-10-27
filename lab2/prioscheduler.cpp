#include "prioscheduler.h"
namespace NYU {
namespace OperatingSystems {
    PRIOScheduler::PRIOScheduler(int quantum, int maxPriority)
    {
        d_quantum = quantum;
        d_priorityMax = maxPriority;
    }
    Process* PRIOScheduler::getProcess()
    {
        return nullptr;
    }
    void PRIOScheduler::addProcess(Process* toSchedule)
    {
    }
}
}