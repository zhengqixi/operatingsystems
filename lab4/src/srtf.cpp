#include "srtf.h"
#include "request.h"
#include "scheduler.h"
#include <limits>
#include <list>
namespace NYU {
namespace OperatingSystems {
    Request* SRTF::getRequest()
    {
        unsigned int smallestDiff = std::numeric_limits<unsigned int>::max();
        auto iter = d_queue.begin();
        for (auto i = d_queue.begin(); i != d_queue.end(); ++i) {
            unsigned int track = (*i)->track();
            unsigned int difference = 0;
            difference = track > d_lastTrackPosition ? track - d_lastTrackPosition : d_lastTrackPosition - track;
            if (difference < smallestDiff) {
                iter = i;
                smallestDiff = difference;
            }
        }
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