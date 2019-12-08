#include "lookbase.h"
#include "request.h"
#include "scheduler.h"
#include <algorithm>
#include <list>
namespace NYU {
namespace OperatingSystems {
    void LookBase::add(std::list<Request*>& queue, Request* newRequest)
    {
        auto iter = std::lower_bound(queue.begin(), queue.end(), newRequest,
            [](const Request* const a, const Request* const b) {
                if (a->track() == b->track()) {
                    return a->arriveTime() < b->arriveTime();
                }
                return a->track() < b->track();
            });
        queue.insert(iter, newRequest);
    }
    Request* LookBase::getRequestFromQueue(std::list<Request*>& queue)
    {
        Request* found = nullptr;
        // Increasing
        unsigned int lastSeek = d_lastSeek;
        if (d_increase) {
            auto iter = std::find_if(queue.begin(), queue.end(), [lastSeek](const Request* const a) {
                if (a->track() >= lastSeek) {
                    return true;
                }
                return false;
            });
            if (iter == queue.end()) {
                d_increase = false;
                return getRequestFromQueue(queue);
            }
            found = *iter;
            queue.erase(iter);
        } else {
            auto iter = std::find_if(queue.rbegin(), queue.rend(), [lastSeek](const Request* const a) {
                if (a->track() <= lastSeek) {
                    return true;
                }
                return false;
            });
            if (iter == queue.rend()) {
                d_increase = true;
                return getRequestFromQueue(queue);
            }
            auto foundTrack = (*iter)->track();
            // We find the first iterator where the foundtrack is differetn
            // This is to find any tied tracks with a lower arrival time
            auto trackTie = std::find_if(iter, queue.rend(), [foundTrack](const Request* const a) {
                return a->track() != foundTrack;
            });
            iter = (--trackTie);
            found = *iter;
            auto fIter = --(iter.base());
            queue.erase(fIter);
        }
        d_lastSeek = found->track();
        return found;
    }
}
}