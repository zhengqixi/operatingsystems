#include "simulation.h"
#include <cassert>
#include <fstream>
#include <memory>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>
namespace NYU {
namespace OperatingSystems {
    Simulation::Simulation(std::ifstream& processFile, std::ifstream& randomFile, std::shared_ptr<AbstractScheduler> scheduler)
        : d_randomGenerator{ randomFile }
        , d_scheduler{ scheduler }
    {
        initializeEventQueue(processFile);
    }
    void Simulation::Simulate(std::ostream& output)
    {
        long quantum = d_scheduler->quantum();
        bool callScheduler = false;
        long currentTime = 0;
        // If a process transitioning to ready preempts the current process,
        // This is set to guard against multiple processes preempting the current process
        // One the scheduler is called, the new process is run and this is no longer true
        bool processBootEvent = false;
        std::shared_ptr<Process> currentProcess = nullptr;
        while (d_eventQueue.hasEvents()) {
            auto event = d_eventQueue.popEvent();
            currentTime = event.timeStamp();
            auto process = event.data();
            long elaspedTime = process->timeStamp() - currentTime;
            assert(elaspedTime >= 0);
            process->setTimeStamp(currentTime);
            switch (process->transition()) {
            case TRANS_READY: {
                callScheduler = true;
                d_scheduler->addProcess(process);
                process->resetDynamicPriority();
                // Check if process will preempt the current running process
                // This only really happens on the preprio
                // Remove events for current running process from queue and generate preempt event for this timestamp
                if (!processBootEvent && d_scheduler->testPrempt(process, currentProcess)) {
                    processBootEvent = true;
                    if (d_eventQueue.removeEvent(currentProcess, currentTime)) {
                        currentProcess->setTransition(TRANS_PREEMPT);
                        d_eventQueue.addEvent(currentTime, currentProcess);
                    }
                }
                break;
            }
            case TRANS_BLOCK: {
                assert(currentProcess == process);
                callScheduler = true;
                currentProcess = nullptr;
                process->runCPU(elaspedTime);
                if (process->totalCPUTime() != 0) {
                    d_scheduler->addProcess(process);
                }
                process->setTransition(TRANS_READY);
                int ioBurst = d_randomGenerator.getRandom(process->ioBurst());
                d_eventQueue.addEvent(currentTime + ioBurst, process);
                break;
            }
            case TRANS_RUN: {
                // Current process must be taken out by a preemption event or something first
                assert(currentProcess == nullptr);
                currentProcess = process;
                int runTime = d_randomGenerator.getRandom(process->cpuBurst());
                if (runTime > process->totalCPUTime()) {
                    runTime = process->totalCPUTime();
                }
                if (runTime > quantum) {
                    process->setTransition(TRANS_PREEMPT);
                    d_eventQueue.addEvent(currentTime + quantum, process);
                } else {
                    process->setTransition(TRANS_BLOCK);
                    d_eventQueue.addEvent(currentTime + runTime, process);
                }
                break;
            }
            case TRANS_PREEMPT: {
                assert(currentProcess == process);
                callScheduler = true;
                currentProcess = nullptr;
                process->runCPU(elaspedTime);
                if (process->totalCPUTime() != 0) {
                    d_scheduler->addProcess(process);
                }
                process->resetDynamicPriority();
                break;
            }
            }
            if (callScheduler) {
                if (d_eventQueue.peekNextTimeStamp() == currentTime) {
                    continue;
                }
                callScheduler = false;
                processBootEvent = false;
                if (currentProcess == nullptr) {
                    auto nextProcess = d_scheduler->getProcess();
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
        std::string line;
        int maxPrio = d_scheduler->maxPriority();
        while (std::getline(processFile, line)) {
            std::stringstream lineParser{ line };
            int timeStamp;
            int cpuTime;
            int cpuBurst;
            int ioBurst;
            lineParser >> timeStamp >> cpuTime >> cpuBurst >> ioBurst;
            int priority = d_randomGenerator.getRandom(maxPrio);
            d_eventQueue.addEvent(timeStamp,
                Process::createProcess(cpuTime, cpuBurst, ioBurst, timeStamp, priority));
        }
    }
}
}