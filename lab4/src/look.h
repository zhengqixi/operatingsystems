#ifndef LOOK_H
#define LOOK_H
#include "lookbase.h"
#include <list>
namespace NYU {
namespace OperatingSystems {
    class Look : public LookBase {
    public:
        Request* getRequest();
        void addToQueue(Request* newRequest);
        bool empty() const;

    private:
        std::list<Request*> d_queue;
    };
}
}

#endif