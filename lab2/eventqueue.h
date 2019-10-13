#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include "event.h"
#include <queue>
#include <vector>
namespace {
using namespace NYU::OperatingSystems;
template <typename T>
class EventComparator {
public:
    bool operator()(const Event<T>& a, const Event<T>& b) const
    {
        if (a.timeStamp() == b.timeStamp()) {
            return a.eventID() > b.eventID();
        }
        return a.timeStamp() > b.timeStamp();
    }
};
}
namespace NYU {
namespace OperatingSystems {
    template <typename T>
    class EventQueue {
    public:
        // Get the top event and remove from queue
        Event<T> popEvent();
        // Look at the top event without removing
        const Event<T>& peekEvent() const;
        // Add a new event
        void addEvent(eTime timeStamp, T data);
        // Test if there are any events remaining in queue
        bool hasEvents() const;

    private:
        eID d_nextValidID = 0;
        std::priority_queue<Event<T>, std::vector<Event<T>>, EventComparator<T>> d_queue;
    };
    template <typename T>
    Event<T> EventQueue<T>::popEvent()
    {
        auto event = d_queue.top();
        d_queue.pop();
        return event;
    }
    template <typename T>
    const Event<T>& EventQueue<T>::peekEvent() const
    {
        return d_queue.top();
    }
    template <typename T>
    void EventQueue<T>::addEvent(eTime timeStamp, T data)
    {
        d_queue.push(Event<T>(d_nextValidID, timeStamp, data));
        ++d_nextValidID;
    }
    template <typename T>
    bool EventQueue<T>::hasEvents() const
    {
        return !d_queue.empty();
    }
}
}
#endif