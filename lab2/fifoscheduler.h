#ifndef FIFO_SCHEDULER_H
#define FIFO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
#include <memory>
#include <queue>
namespace NYU {
namespace OperatingSystems {
    class FifoScheduler : AbstractScheduler {
    public:
        virtual std::shared_ptr<Process> getProcess() override;
        // Adds a process to be scheduled eventually
        virtual void addProcess(std::shared_ptr<Process> toSchedule) override;

    private:
        std::queue<std::shared_ptr<Process>> d_queue;
    };
}
}
#endif