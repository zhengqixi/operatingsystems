#include "srtf.h"
#include "request.h"
#include "scheduler.h"
#include <algorithm>
#include <limits>
#include <list>
namespace NYU {
namespace OperatingSystems {
    Request* SRTF::getRequest()
    {
        unsigned int lastPosition = d_lastTrackPosition;
        auto iter = std::min_element(d_queue.begin(), d_queue.end(),
            [lastPosition](const Request* const a, const Request* const b) {
                unsigned int differenceA = a->track() > lastPosition ? a->track() - lastPosition : lastPosition - a->track();
                unsigned int differenceB = b->track() > lastPosition ? b->track() - lastPosition : lastPosition - b->track();
                if (differenceA == differenceB) {
                    return a->arriveTime() < b->arriveTime();
                }
                return differenceA < differenceB;
            });
        d_queue.erase(iter);
        Request* selected = *iter;
        d_lastTrackPosition = selected->track();
        return selected;
    }
    void SRTF::addToQueue(Request* newRequest)
    {
        d_queue.push_back(newRequest);
    }
    bool SRTF::empty() const
    {
        return d_queue.empty();
    }
}
}