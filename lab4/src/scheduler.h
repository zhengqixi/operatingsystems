#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "request.h"
namespace NYU {
namespace OperatingSystems {
    class Scheduler {
    public:
        virtual Request* getRequest(unsigned int headPosition) = 0;
        virtual void addToQueue(Request* newRequest) = 0;
        virtual bool empty() const = 0;
    };
}
}
#endif