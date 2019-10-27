#ifndef PRE_PRIO_SCHEDULER_H
#define PRE_PRIO_SCHEDULER_H
#include "prioscheduler.h"
#include "process.h"
namespace NYU {
namespace OperatingSystems {
    class PREPRIOScheduler : public PRIOScheduler {
    public:
        PREPRIOScheduler(int quantum, int maxPriority);
        bool testPrempt(Process* newlyArrived, Process* currentProcess) const;

    private:
    };
}
}
#endif