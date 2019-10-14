#include "simulation.h"
#include <fstream>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
namespace NYU {
namespace OperatingSystems {
    Simulation::Simulation(std::ifstream& processFile, std::ifstream& randomFile, AbstractScheduler& scheduler)
        : d_randomGenerator{ randomFile }
        , d_scheduler{ scheduler }
    {
        initializeEventQueue(processFile);
    }
    void Simulation::Simulate(std::ostream& output)
    {
        bool callScheduler = false;
        eTime currentTime = 0;
        while (d_eventQueue.hasEvents()) {
            auto process = d_eventQueue.popEvent().data();
            switch (process->getState()) {
            case CREATED:
                break;
            case READY:
                break;
            case RUNNING:
                break;
            case BLOCKED:
                break;
            }
            d_scheduler.addProcess(process);
            if (callScheduler) {
                if (d_eventQueue.peekNextTimeStamp() == currentTime) {
                    continue;
                }
            }
        }
    }
    void Simulation::initializeEventQueue(std::ifstream& processFile)
    {
        using namespace std;
        string line;
        while (getline(processFile, line)) {
            stringstream lineParser{ line };
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