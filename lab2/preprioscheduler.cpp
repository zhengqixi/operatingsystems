#include "preprioscheduler.h"
#include "prioscheduler.h"
#include "process.h"
namespace NYU {
namespace OperatingSystems {
    PREPRIOScheduler::PREPRIOScheduler(int quantum, int maxPriority)
        : PRIOScheduler(quantum, maxPriority)
    {
    }
    bool PREPRIOScheduler::testPrempt(Process* newlyArrived, Process* currentProcess) const
    {
        return newlyArrived->dynamicPriority() > currentProcess->dynamicPriority();
    }
}
}