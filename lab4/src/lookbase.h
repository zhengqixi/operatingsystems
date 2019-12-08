#ifndef LOOK_BASE_H
#define LOOK_BASE_H
#include "scheduler.h"
#include <list>
namespace NYU {
namespace OperatingSystems {
    class LookBase : public Scheduler {
    protected:
        Request* getRequestFromQueue(std::list<Request*>& queue);
        void add(std::list<Request*>& queue, Request* newRequest);
        unsigned int d_lastSeek = 0;
        // Direction
        bool d_increase = true;
    };
}
}
#endif