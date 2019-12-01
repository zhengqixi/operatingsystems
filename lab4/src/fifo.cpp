#include "fifo.h"
#include "request.h"
namespace NYU {
namespace OperatingSystems {
    Request* Fifo::getRequest()
    {
        auto ptr = d_queue.front();
        d_queue.pop();
        return ptr;
    }
    void Fifo::addToQueue(Request* newRequest)
    {
        d_queue.push(newRequest);
    }
    bool Fifo::empty() const
    {
        return d_queue.empty();
    }
}
}