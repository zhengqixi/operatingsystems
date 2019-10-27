#ifndef PRIO_SCHEDULER_H
#define PRIO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
namespace NYU {
namespace OperatingSystems {
    class PRIOScheduler : public AbstractScheduler {
    public:
        PRIOScheduler(int quantum, int maxPriority);
        Process* getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(Process* toSchedule);

    private:
    };
}
}
#endif