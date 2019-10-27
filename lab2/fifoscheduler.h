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
        std::shared_ptr<Process> getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(std::shared_ptr<Process> toSchedule);

    private:
        std::queue<std::shared_ptr<Process>> d_queue;
    };
}
}
#endif