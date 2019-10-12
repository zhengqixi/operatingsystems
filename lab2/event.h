#ifndef EVENT_H
#define EVENT_H
namespace NYU {
namespace OperatingSystems {
    typedef unsigned long long eID;
    typedef unsigned int eTime;
    template <typename T>
    class Event {
    public:
        Event(eID id, eTime timeStamp, T data);
        eID EventID() const;
        eTime TimeStamp() const;
        T Data() const;

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
    eID Event<T>::EventID() const
    {
        return d_eventID;
    }
    template <typename T>
    eTime Event<T>::TimeStamp() const
    {
        return d_timeStamp;
    }
    template <typename T>
    T Event<T>::Data() const
    {
        return d_data;
    }
}
}
#endif