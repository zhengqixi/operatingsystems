#ifndef ABSTRACT_SCHEDULER_H
#define ABSTRACT_SCHEDULER_H
#include "process.h"
#include <memory>
namespace NYU {
namespace OperatingSystems {
    class AbstractScheduler {
    public:
        // Gets a process to run
        virtual std::shared_ptr<Process> getProcess() = 0;
        // Adds a process to be scheduled eventually
        virtual void addProcess(std::shared_ptr<Process> toSchedule) = 0;
        // Get the quantum
        long quantum() const
        {
            return d_quantum;
        }
        // Test if a newly ready to run process should preempt the current running one
        virtual bool testPrempt(std::shared_ptr<Process> newlyArrived, std::shared_ptr<Process> currentProcess) const
        {
            return false;
        }
        // get the maximum priority supported
        int maxPriority() const
        {
            return d_priorityMax;
        }

    protected:
        long d_quantum = 10000;
        int d_priorityMax = 4;
    };
}
}
#endif