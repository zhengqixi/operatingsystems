#ifndef LIFO_SCHEDULER_H
#define LIFO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"

#include <stack>
namespace NYU {
namespace OperatingSystems {
    class LIFOScheduler : public AbstractScheduler {
    public:
        virtual Process* getProcess() override;
        // Adds a process to be scheduled eventually
        virtual void addProcess(Process* toSchedule) override;

    private:
        std::stack<Process*> d_stack;
    };
}
}
#endif