#ifndef SIMULATION_H
#define SIMULATION_H
#include "abstractscheduler.h"
#include "eventqueue.h"
#include "process.h"
#include "randomgenerator.h"
#include <fstream>
#include <ostream>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class Simulation {
    public:
        // Simulation objects takes ownership of AbstractScheduler pointer
        Simulation(std::ifstream& processFile, std::ifstream& randomFile, AbstractScheduler* scheduler);
        // Simulates the events, logging output to the provided output stream
        void Simulate(std::ostream& output, bool verbose);
        friend std::ostream& operator<<(std::ostream& out, const Simulation& simulation);
        ~Simulation();

    private:
        AbstractScheduler* d_scheduler;
        void initializeEventQueue(std::ifstream& processFile);
        EventQueue<Process*> d_eventQueue;
        RandomGenerator d_randomGenerator;
        std::vector<Process*> d_processList;
        // Runs the process for the elaspedTime. Returns true if process is terminated
        bool checkTermination(Process* process, int elaspedTime, int currentTime, std::ostream& output, bool verbose);
        void verboseHeader(std::ostream& output, Process* process, int currentTime, int processElaspedTime);
        // Simulation state data
        // Summarization data
        int d_finishTime = 0;
        double d_ioUsagePercentage = 0;
        double d_cpuUsagePercentage = 0;
        double d_processThroughput = 0;
        double d_averageCPUWaitTime = 0;
        double d_averageCPUTurnaroundTime = 0;
    };
}
}
#endif