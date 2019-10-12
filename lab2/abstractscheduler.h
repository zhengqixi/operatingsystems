#ifndef ABSTRACT_SCHEDULER_H
#define ABSTRACT_SCHEDULER_H
#include "process.h"
#include <memory>
namespace NYU {
namespace OperatingSystems {
    class AbstractScheduler {
        // Gets a process to run
        virtual std::unique_ptr<Process> getProcess() = 0;
        // Adds a process to be scheduled eventually
        virtual void addProcess(std::unique_ptr<Process> toSchedule) = 0;
    };
}
}
#endif