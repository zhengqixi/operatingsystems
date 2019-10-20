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
        long quantum() const
        {
            return d_quantum;
        }
        virtual bool testPrempt(std::shared_ptr<Process> newlyArrived, long currTime) = 0;

    protected:
        long d_quantum = 10000;
    };
}
}
#endif