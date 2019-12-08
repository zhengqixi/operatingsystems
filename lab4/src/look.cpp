#include "look.h"
#include "lookbase.h"
#include <list>
namespace NYU {
namespace OperatingSystems {
    Request* Look::getRequest()
    {
        return getRequestFromQueue(d_queue);
    }
    void Look::addToQueue(Request* newRequest)
    {
        add(d_queue, newRequest);
    }
    bool Look::empty() const
    {
        return d_queue.empty();
    }
}
}