#include "srtfscheduler.h"
#include "process.h"
namespace NYU {
namespace OperatingSystems {
    Process* SRTFScheduler::getProcess()
    {
        if (d_queue.empty()) {
            return nullptr;
        }
        auto newProcess = d_queue.top();
        d_queue.pop();
        return newProcess;
    }
    void SRTFScheduler::addProcess(Process* toSchedule)
    {
        d_queue.push(toSchedule);
    }
}
}