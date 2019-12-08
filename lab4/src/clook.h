#ifndef CLOOK_H
#define CLOOK_H
#include "request.h"
#include "scheduler.h"
#include <list>
namespace NYU {
namespace OperatingSystems {
    class CLook : public Scheduler {
    public:
        Request* getRequest();
        void addToQueue(Request* newRequest);
        bool empty() const;

    private:
        unsigned int d_lastSeek = 0;
        std::list<Request*> d_queue;
    };
}
}
#endif