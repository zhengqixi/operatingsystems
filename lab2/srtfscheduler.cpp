#include "srtfscheduler.h"
#include "process.h"
namespace NYU {
namespace OperatingSystems {
    Process* SRTFScheduler::getProcess()
    {
        if (!d_queue.hasEvents()) {
            return nullptr;
        }
        auto nextProcess = d_queue.popEvent();
        return nextProcess.data();
    }
    void SRTFScheduler::addProcess(Process* toSchedule)
    {
        d_queue.addEvent(toSchedule->remainingCPUTime(), toSchedule);
    }
}
}