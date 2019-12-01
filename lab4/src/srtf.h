#ifndef SRTF_H
#define SRTF_H
#include "request.h"
#include "scheduler.h"
#include <list>
namespace NYU {
namespace OperatingSystems {
    class SRTF : public Scheduler {
    public:
        Request* getRequest();
        void addToQueue(Request* newRequest);
        bool empty() const;

    private:
        unsigned int d_lastTrackPosition = 0;
        std::list<Request*> d_queue;
    };
}
}
#endif