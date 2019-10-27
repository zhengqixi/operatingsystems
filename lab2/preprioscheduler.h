#ifndef PRE_PRIO_SCHEDULER_H
#define PRE_PRIO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
namespace NYU {
namespace OperatingSystems {
    class PREPRIOScheduler : public AbstractScheduler {
    public:
        PREPRIOScheduler(int quantum, int maxPriority);
        Process* getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(Process* toSchedule);

    private:
    };
}
}
#endif