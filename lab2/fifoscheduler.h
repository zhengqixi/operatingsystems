#ifndef FIFO_SCHEDULER_H
#define FIFO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
#include <queue>
namespace NYU {
namespace OperatingSystems {
    class FIFOScheduler : public AbstractScheduler {
    public:
        FIFOScheduler();
        // Overwriting the quantum to a reasonable value turns this into a round robin scheduler
        // Because Nisarg said that conceptually they're the same thing
        FIFOScheduler(int quantum);
        Process* getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(Process* toSchedule);

    private:
        std::queue<Process*> d_queue;
    };
}
}
#endif