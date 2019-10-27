#ifndef SRTF_SCHEDULER_H
#define SRTF_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
#include <queue>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class SRTFScheduler : public AbstractScheduler {
    public:
        Process* getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(Process* toSchedule);

    private:
        class ShortestRemainingTimeComparator {
        public:
            bool operator()(const Process* const a, const Process* const b) const
            {
                return a->remainingCPUTime() > b->remainingCPUTime();
            }
        };
        std::priority_queue<Process*, std::vector<Process*>, ShortestRemainingTimeComparator> d_queue;
    };
}
}
#endif