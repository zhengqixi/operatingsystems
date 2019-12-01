#ifndef REQUEST_H
#define REQUEST_H
#include <ostream>
namespace NYU {
namespace OperatingSystems {
    class Request {
    public:
        Request(unsigned int id, unsigned int arriveTime, unsigned int track);
        unsigned int arriveTime() const;
        unsigned int track() const;
        unsigned int& startTime();
        unsigned int& endTime();
        friend std::ostream& operator<<(std::ostream& output, const Request& request);

    private:
        unsigned int d_arriveTime;
        unsigned int d_track;
        unsigned int d_id;
        unsigned int d_startTime = 0;
        unsigned int d_endTime = 0;
    };
}
}
#endif