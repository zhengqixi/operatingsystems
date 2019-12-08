#ifndef FLOOK_H
#define FLOOK_H
#include "lookbase.h"
#include "request.h"
#include <list>
namespace NYU {
namespace OperatingSystems {
    class FLook : public LookBase {
    public:
        Request* getRequest();
        void addToQueue(Request* newRequest);
        bool empty() const;

    private:
        std::list<Request*> d_activeQueue;
        std::list<Request*> d_addQueue;
    };
}
}

#endif