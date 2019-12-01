#include "request.h"
namespace NYU {
namespace OperatingSystems {
    Request::Request(unsigned int id, unsigned int arriveTime, unsigned int track)
        : d_id(id)
        , d_arriveTime(arriveTime)
        , d_track(track)
    {
    }
    unsigned int Request::arriveTime()
    {
        return d_arriveTime;
    }
    unsigned int Request::track()
    {
        return d_track;
    }
    unsigned int& Request::startTime()
    {
        return d_startTime;
    }
    unsigned int& Request::endTime()
    {
        return d_endTime;
    }
}
}