#ifndef REQUEST_H
#define REQUEST_H
namespace NYU {
namespace OperatingSystems {
    class Request {
    public:
        Request(unsigned int id, unsigned int arriveTime, unsigned int track);
        unsigned int arriveTime();
        unsigned int track();
        unsigned int& startTime();
        unsigned int& endTime();

    private:
        unsigned int d_arriveTime;
        unsigned int d_track;
        unsigned int d_id;
        unsigned int d_startTime;
        unsigned int d_endTime;
    };
}
}
#endif