#include "simulation.h"
#include <cassert>
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
        long currentTime = 0;
        std::shared_ptr<Process> currentProcess = nullptr;
        while (d_eventQueue.hasEvents()) {
            auto event = d_eventQueue.popEvent();
            currentTime = event.timeStamp();
            auto process = event.data();
            switch (process->transition()) {
            case TRANS_READY:
                d_scheduler.addProcess(process);
                // Check if process will preempt the current running process
                // Remove events for current running process from queue and generate preempt event for this timestamp
                callScheduler = true;
                break;
            case TRANS_BLOCK:
                assert(currentProcess == process);
                currentProcess = nullptr;
                // Check CPU time first for termination
                // Then update CPU time
                // Generate transition to ready event
                process->setTransition(TRANS_READY);
                callScheduler = true;
                break;
            case TRANS_RUN:
                currentProcess = process;
                break;
            case TRANS_PREEMPT:
                assert(currentProcess == process);
                currentProcess = nullptr;
                // Check CPU time first for termination
                // Then update CPU time
                d_scheduler.addProcess(process);
                callScheduler = true;
                break;
            }
            if (callScheduler) {
                if (d_eventQueue.peekNextTimeStamp() == currentTime) {
                    continue;
                }
                callScheduler = false;
                if (currentProcess == nullptr) {
                    auto nextProcess = d_scheduler.getProcess();
                    if (nextProcess != nullptr) {
                        nextProcess->setTransition(TRANS_RUN);
                        d_eventQueue.addEvent(currentTime, nextProcess);
                    }
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