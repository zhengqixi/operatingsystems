#include "flook.h"
#include "lookbase.h"
#include "request.h"
#include <list>
namespace NYU {
namespace OperatingSystems {
    Request* FLook::getRequest()
    {
        if (d_activeQueue.empty()) {
            d_activeQueue.swap(d_addQueue);
        }
        return getRequestFromQueue(d_activeQueue);
    }
    void FLook::addToQueue(Request* newRequest)
    {
        add(d_addQueue, newRequest);
    }
    bool FLook::empty() const
    {
        return d_addQueue.empty() && d_activeQueue.empty();
    }
}
}
