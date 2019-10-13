#ifndef SIMULATION_H
#define SIMULATION_H
#include "abstractscheduler.h"
#include "eventqueue.h"
#include "process.h"
#include "randomgenerator.h"
#include <fstream>
#include <memory>
#include <ostream>
namespace NYU {
namespace OperatingSystems {
    class Simulation {
    public:
        Simulation(std::ifstream& processFile, std::ifstream& randomFile, AbstractScheduler& scheduler);
        // Simulates the events, logging output to the provided output stream
        void Simulate(std::ostream& output);

    private:
        AbstractScheduler& d_scheduler;
        void initializeEventQueue(std::ifstream& processFile);
        EventQueue<std::shared_ptr<Process>> d_eventQueue;
        RandomGenerator d_randomGenerator;
        eTime d_currTime = 0;
        PID d_currProcess = -1;
    };
}
}
#endif