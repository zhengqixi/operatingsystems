#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H
#include <algorithm>
#include <queue>
#include <utility>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    template <typename T>
    class Event {
    public:
        Event(int id, int timeStamp, T data);
        // Get the event ID
        int eventID() const;
        // Get the event time stamp
        int timeStamp() const;
        // Get the event data
        const T& data() const;

    private:
        int d_eventID;
        int d_timeStamp;
        T d_data;
    };
    template <typename T>
    Event<T>::Event(int id, int timeStamp, T data)
        : d_eventID{ d_eventID }
        , d_timeStamp{ timeStamp }
        , d_data{ data }
    {
    }
    template <typename T>
    int Event<T>::eventID() const
    {
        return d_eventID;
    }
    template <typename T>
    int Event<T>::timeStamp() const
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
        int peekNextTimeStamp() const;
        // Add a new event
        void addEvent(int timeStamp, T data);
        // Test if there are any events remaining in queue
        bool hasEvents() const;
        // Remove an event for T data, where the time IS NOT the currentTime passed in
        // Returns true if an event has been removed. False otherwise
        bool removeEvent(const T& data, int currentTime);

    private:
        int d_nextValidID = 0;
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
    int EventQueue<T>::peekNextTimeStamp() const
    {
        if (d_queue.empty()) {
            return -1;
        }
        return d_queue.front().timeStamp();
    }
    template <typename T>
    void EventQueue<T>::addEvent(int timeStamp, T data)
    {
        d_queue.push_back(std::move(Event<T>(d_nextValidID, timeStamp, std::move(data))));
        std::push_heap(d_queue.begin(), d_queue.end(), EventComparator<T>());
        ++d_nextValidID;
    }
    template <typename T>
    bool EventQueue<T>::hasEvents() const
    {
        return !d_queue.empty();
    }
    template <typename T>
    bool EventQueue<T>::removeEvent(const T& data, int currentTime)
    {
        auto found = std::find_if(d_queue.begin(), d_queue.end(), [data, currentTime](const Event<T>& match) {
            return match.timeStamp() != currentTime && match.data() == data;
        });
        if (found == d_queue.end()) {
            return false;
        }
        std::iter_swap(found, d_queue.end() - 1);
        d_queue.pop_back();
        std::make_heap(found, d_queue.end(), EventComparator<T>());
        return true;
    }
}
}
#endif