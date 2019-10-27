#ifndef LIFO_SCHEDULER_H
#define LIFO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
#include <memory>
#include <stack>
namespace NYU {
namespace OperatingSystems {
    class LifoScheduler : public AbstractScheduler {
    public:
        virtual std::shared_ptr<Process> getProcess() override;
        // Adds a process to be scheduled eventually
        virtual void addProcess(std::shared_ptr<Process> toSchedule) override;

    private:
        std::stack<std::shared_ptr<Process>> d_stack;
    };
}
}
#endif