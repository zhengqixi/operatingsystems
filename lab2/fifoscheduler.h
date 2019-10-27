#ifndef FIFO_SCHEDULER_H
#define FIFO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
#include <memory>
#include <queue>
namespace NYU {
namespace OperatingSystems {
    class FifoScheduler : public AbstractScheduler {
    public:
        Process* getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(Process* toSchedule);

    private:
        std::queue<Process*> d_queue;
    };
}
}
#endif