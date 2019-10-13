#include "simulation.h"
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
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
            auto process = data.data();
            output << "Timestamp: " << data.timeStamp() << " ";
            output << "PID: " << process->pid() << " ";
            output << '\n';
        }
    }
    void Simulation::initializeEventQueue(std::ifstream& processFile)
    {
        using namespace std;
        string line;
        while (getline(processFile, line)) {
            stringstream lineParser(line);
            string timestamp;
            string cpuTime;
            string cpuBurst;
            string ioBurst;
            lineParser >> timestamp >> cpuTime >> cpuBurst >> ioBurst;
            d_eventQueue.addEvent(stoi(timestamp),
                std::move(Process::createProcess(stoi(cpuTime), stoi(cpuBurst), stoi(ioBurst))));
        }
    }
}
}