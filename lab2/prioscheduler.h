#ifndef PRIO_SCHEDULER_H
#define PRIO_SCHEDULER_H
#include "abstractscheduler.h"
#include "process.h"
#include <queue>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class PRIOScheduler : public AbstractScheduler {
    public:
        PRIOScheduler(int quantum, int maxPriority);
        Process* getProcess();
        // Adds a process to be scheduled eventually
        void addProcess(Process* toSchedule);

    private:
        class DecayQueue {
        public:
            DecayQueue(int maxPriority);
            bool empty() const;
            void addProcess(Process* process);
            Process* getProcess();

        private:
            std::vector<std::queue<Process*>> d_decayQueue;
            int d_firstSetBit = 0;
            int d_processCount = 0;
        };
        DecayQueue d_active;
        DecayQueue d_expired;
    };
}
}
#endif