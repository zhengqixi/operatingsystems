#ifndef SIMULATION_H
#define SIMULATION_H
#include "scheduler.h"
#include <istream>
#include <ostream>
#include <queue>
#include <string>
namespace NYU {
namespace OperatingSystems {
    class Simulation {
    public:
        Simulation(std::istream& input, Scheduler* scheduler);
        ~Simulation();
        void run(std::ostream& output);

    private:
        Scheduler* d_scheduler;
        std::queue<Request*> d_notArrived;
        std::vector<Request*> d_requests;
        std::string nextLine(std::istream& input) const;
    };
}
}
#endif