#ifndef SIMULATION_H
#define SIMULATION_H
#include "abstractscheduler.h"
#include "eventqueue.h"
#include "process.h"
#include "randomgenerator.h"
#include <fstream>
#include <memory>
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class Simulation {
    public:
        Simulation(std::ifstream& processFile, std::ifstream& randomFile, std::shared_ptr<AbstractScheduler> scheduler);
        // Simulates the events, logging output to the provided output stream
        void Simulate(std::ostream& output, bool verbose);
        friend std::ostream& operator<<(std::ostream& out, const Simulation& simulation);
        ~Simulation();

    private:
        std::shared_ptr<AbstractScheduler> d_scheduler;
        void initializeEventQueue(std::ifstream& processFile);
        EventQueue<Process*> d_eventQueue;
        RandomGenerator d_randomGenerator;
        std::vector<Process*> d_processList;
        // Runs the process for the elaspedTime. Returns true if process is terminated
        // Otherwise adds to the scheduler
        bool checkTermination(Process* process, int elaspedTime, int currentTime, std::ostream& output, bool verbose);
        void verboseHeader(std::ostream& output, Process* process, int currentTime);
    };
}
}
#endif