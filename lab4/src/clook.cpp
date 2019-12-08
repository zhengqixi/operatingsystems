#include "clook.h"
#include "scheduler.h"
#include <algorithm>
namespace NYU {
namespace OperatingSystems {
    Request* CLook::getRequest()
    {
        Request* found = nullptr;
        while (found == nullptr) {
            unsigned int lastSeek = d_lastSeek;
            auto iter = std::find_if(d_queue.begin(), d_queue.end(), [lastSeek](const Request* const a) {
                if (a->track() >= lastSeek) {
                    return true;
                }
                return false;
            });
            if (iter == d_queue.end()) {
                d_lastSeek = 0;
                continue;
            }
            found = *iter;
            d_queue.erase(iter);
        }
        d_lastSeek = found->track();
        return found;
    }
    void CLook::addToQueue(Request* newRequest)
    {
        auto iter = std::lower_bound(d_queue.begin(), d_queue.end(), newRequest,
            [](const Request* const a, const Request* const b) {
                if (a->track() == b->track()) {
                    return a->arriveTime() < b->arriveTime();
                }
                return a->track() < b->track();
            });
        d_queue.insert(iter, newRequest);
    }
    bool CLook::empty() const
    {
        return d_queue.empty();
    }
}
}