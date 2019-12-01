#ifndef FIFO_H
#define FIFO_H
#include "request.h"
#include "scheduler.h"
#include <queue>
namespace NYU {
namespace OperatingSystems {
    class Fifo : public Scheduler {
    public:
        Request* getRequest(unsigned int headPosition);
        void addToQueue(Request* newRequest);
        bool empty() const;

    private:
        std::queue<Request*> d_queue;
    };
}
}
#endif