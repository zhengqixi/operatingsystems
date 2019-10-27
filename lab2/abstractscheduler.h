#ifndef ABSTRACT_SCHEDULER_H
#define ABSTRACT_SCHEDULER_H
#include "process.h"
#include <memory>
namespace NYU {
namespace OperatingSystems {
    class AbstractScheduler {
    public:
        // Gets a process to run
        virtual Process* getProcess() = 0;
        // Adds a process to be scheduled eventually
        virtual void addProcess(Process* toSchedule) = 0;
        // Get the quantum
        int quantum() const
        {
            return d_quantum;
        }
        // Test if a newly ready to run process should preempt the current running one
        virtual bool testPrempt(Process* newlyArrived, Process* currentProcess) const
        {
            return false;
        }
        // get the maximum priority supported
        int maxPriority() const
        {
            return d_priorityMax;
        }

    protected:
        int d_quantum = 10000;
        int d_priorityMax = 4;
    };
}
}
#endif