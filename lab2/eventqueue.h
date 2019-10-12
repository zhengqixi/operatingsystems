#include "event.h"
#include <queue>
#include <vector>
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
namespace {
using namespace NYU::OperatingSystems;
template <typename T>
class EventComparator {
public:
    bool operator()(const Event<T>& a, const Event<T>& b) const
    {
        if (a.TimeStamp() == b.TimeStamp()) {
            return a.EventID() > b.EventID();
        }
        return a.TimeStamp() > b.TimeStamp();
    }
};
}
namespace NYU {
namespace OperatingSystems {
    template <typename T>
    class EventQueue {
    public:
        Event<T> PopEvent();
        const Event<T>& PeekEvent() const;
        void AddEvent(eTime timeStamp, T data);
        bool Continue() const;

    private:
        eID d_nextValidID = 0;
        std::priority_queue<Event<T>, std::vector<Event<T>>, EventComparator<T>> d_queue;
    };
    template <typename T>
    Event<T> EventQueue<T>::PopEvent()
    {
        auto event = d_queue.top();
        d_queue.pop();
        return event;
    }
    template <typename T>
    const Event<T>& EventQueue<T>::PeekEvent() const
    {
        return d_queue.top();
    }
    template <typename T>
    void EventQueue<T>::AddEvent(eTime timeStamp, T data)
    {
        d_queue.push(Event<T>(d_nextValidID, timeStamp, data));
        ++d_nextValidID;
    }
    template <typename T>
    bool EventQueue<T>::Continue() const
    {
        return !d_queue.empty();
    }
}
}
#endif