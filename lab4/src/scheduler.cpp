#include "scheduler.h"
namespace NYU {
namespace OperatingSystems {
    void Scheduler::addToQueue(Request* newRequest)
    {
        ++d_currentRequests;
    }
    bool Scheduler::empty() const
    {
        return d_currentRequests == 0;
    }
}
}