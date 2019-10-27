#ifndef SRTF_SCHEDULER_H
#define SRTF_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
namespace NYU {
namespace OperatingSystems {
    class SRTFScheduler : public AbstractScheduler {
    public:
        Process* getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(Process* toSchedule);

    private:
    };
}
}
#endif