#include "simulation.h"
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
namespace NYU {
namespace OperatingSystems {
    Simulation::Simulation(std::ifstream& processFile, std::ifstream& randomFile, AbstractScheduler& scheduler)
        : d_randomGenerator(randomFile)
        , d_scheduler(scheduler)
    {
        initializeEventQueue(processFile);
    }
    void Simulation::Simulate(std::ostream& output)
    {
        while (d_eventQueue.hasEvents()) {
            auto data = d_eventQueue.popEvent();
        }
    }
    void Simulation::initializeEventQueue(std::ifstream& processFile)
    {
        std::string line;
        while (std::getline(processFile, line)) {
            std::stringstream lineParser(line);
            std::string timestamp;
            std::string cpuTime;
            std::string cpuBurst;
            std::string ioBurst;
        }
    }
}
}