#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include <algorithm>
#include <queue>
#include <utility>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    typedef unsigned long long eID;
    typedef unsigned int eTime;
    template <typename T>
    class Event {
    public:
        Event(eID id, eTime timeStamp, T data);
        // Get the event ID
        eID eventID() const;
        // Get the event time stamp
        eTime timeStamp() const;
        // Get the event data
        const T& data() const;

    private:
        eID d_eventID;
        eTime d_timeStamp;
        T d_data;
    };
    template <typename T>
    Event<T>::Event(eID id, eTime timeStamp, T data)
        : d_eventID(d_eventID)
        , d_timeStamp(timeStamp)
        , d_data(data)
    {
    }
    template <typename T>
    eID Event<T>::eventID() const
    {
        return d_eventID;
    }
    template <typename T>
    eTime Event<T>::timeStamp() const
    {
        return d_timeStamp;
    }
    template <typename T>
    const T& Event<T>::data() const
    {
        return d_data;
    }
    template <typename T>
    class EventQueue {
    public:
        // Get the top event and remove from queue
        Event<T> popEvent();
        // Look at the next event timestamp
        eTime peekNextTimeStamp() const;
        // Add a new event
        void addEvent(eTime timeStamp, T data);
        // Test if there are any events remaining in queue
        bool hasEvents() const;

    private:
        eID d_nextValidID = 0;
        template <typename InnerT>
        class EventComparator {
        public:
            bool operator()(const Event<InnerT>& a, const Event<InnerT>& b) const
            {
                if (a.timeStamp() == b.timeStamp()) {
                    return a.eventID() > b.eventID();
                }
                return a.timeStamp() > b.timeStamp();
            }
        };
        std::vector<Event<T>> d_queue;
    };
    template <typename T>
    Event<T> EventQueue<T>::popEvent()
    {
        std::pop_heap(d_queue.begin(), d_queue.end(), EventComparator<T>());
        auto event = std::move(d_queue.back());
        d_queue.pop_back();
        return event;
    }
    template <typename T>
    eTime EventQueue<T>::peekNextTimeStamp() const
    {
        return d_queue.front().timeStamp();
    }
    template <typename T>
    void EventQueue<T>::addEvent(eTime timeStamp, T data)
    {
        d_queue.push_back(Event<T>(d_nextValidID, timeStamp, std::move(data)));
        std::push_heap(d_queue.begin(), d_queue.end(), EventComparator<T>());
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