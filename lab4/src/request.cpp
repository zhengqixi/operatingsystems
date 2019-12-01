#include "request.h"
#include <stdio.h>
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
    std::ostream& operator<<(std::ostream& output, const Request& request)
    {
        char buffer[256];
        sprintf(buffer, "%5d: %5d %5d %5d\n", request.d_id, request.d_arriveTime, request.d_startTime, request.d_endTime);
        output << buffer;
        return output;
    }
}
}