#include "simulation.h"
#include <cassert>
#include <fstream>
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <utility>
namespace NYU {
namespace OperatingSystems {
    Simulation::Simulation(std::ifstream& processFile, std::ifstream& randomFile, AbstractScheduler* scheduler)
        : d_randomGenerator{ randomFile }
        , d_scheduler{ scheduler }
    {
        initializeEventQueue(processFile);
    }
    Simulation::~Simulation()
    {
        for (auto process : d_processList) {
            delete process;
        }
        delete d_scheduler;
    }
    void Simulation::Simulate(std::ostream& output, bool verbose)
    {
        int quantum = d_scheduler->quantum();
        bool callScheduler = false;
        int currentTime = 0;
        int blockedProcesses = 0;
        int ioUsageTime = 0;
        int cpuUsageTime = 0;
        // If a process transitioning to ready preempts the current process,
        // This is set to guard against multiple processes preempting the current process
        // One the scheduler is called, the new process is run and this is no inter true
        bool processBootEvent = false;
        Process* currentProcess = nullptr;
        while (d_eventQueue.hasEvents()) {
            // Really check to see that we don't go below 0
            assert(blockedProcesses >= 0);
            auto event = d_eventQueue.popEvent();
            // Do data recording if we're moving to a new timestamp ONLY
            if (event.timeStamp() != currentTime) {
                // Time between last time stamp and new event
                int elaspsedTime = event.timeStamp() - currentTime;
                // MUST be greater than 0 based on our check
                assert(elaspsedTime > 0);
                if (currentProcess != nullptr) {
                    cpuUsageTime += elaspsedTime;
                }
                if (blockedProcesses > 0) {
                    ioUsageTime += elaspsedTime;
                }
            }
            currentTime = event.timeStamp();
            auto process = event.data();
            int elaspedTime = currentTime - process->timeStamp();
            assert(elaspedTime >= 0);
            process->setTimeStamp(currentTime);
            switch (process->transition()) {
            case TRANS_READY: {
                callScheduler = true;
                if (process->createTime() != currentTime) {
                    --blockedProcesses;
                    if (verbose) {
                        verboseHeader(output, process, currentTime, elaspedTime);
                        output << "BLOCK -> READY\n";
                    }
                    process->addBlockedTime(elaspedTime);
                } else if (verbose) {
                    verboseHeader(output, process, currentTime, elaspedTime);
                    output << "CREATED -> READY\n";
                }
                process->resetDynamicPriority();
                d_scheduler->addProcess(process);
                // Check if process will preempt the current running process
                // This only really happens on the preprio
                // Remove events for current running process from queue and generate preempt event for this timestamp
                if (currentProcess != nullptr && !processBootEvent && d_scheduler->testPrempt(process, currentProcess)) {
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
                if (checkTermination(process, elaspedTime, currentTime, output, verbose)) {
                    break;
                }
                process->setTransition(TRANS_READY);
                int ioBurst = d_randomGenerator.getRandom(process->ioBurst());
                d_eventQueue.addEvent(currentTime + ioBurst, process);
                if (verbose) {
                    verboseHeader(output, process, currentTime, elaspedTime);
                    output << "RUNNG -> BLOCK ib=";
                    output << ioBurst << " rem=";
                    output << process->remainingCPUTime();
                    output << '\n';
                }
                ++blockedProcesses;
                break;
            }
            case TRANS_RUN: {
                // Current process must be taken out by a preemption event or something first
                assert(currentProcess == nullptr);
                process->addWaitingTime(elaspedTime);
                currentProcess = process;
                int runTime = process->remainingCPUBurst();
                if (runTime == 0) {
                    runTime = d_randomGenerator.getRandom(process->cpuBurst());
                }
                if (runTime > process->remainingCPUTime()) {
                    runTime = process->remainingCPUTime();
                }
                process->setCPUBurst(runTime);
                if (runTime > quantum) {
                    process->setTransition(TRANS_PREEMPT);
                    d_eventQueue.addEvent(currentTime + quantum, process);
                } else {
                    process->setTransition(TRANS_BLOCK);
                    d_eventQueue.addEvent(currentTime + runTime, process);
                }
                if (verbose) {
                    verboseHeader(output, process, currentTime, elaspedTime);
                    output << "READY -> RUNNG cb=";
                    output << runTime << " rem=";
                    output << process->remainingCPUTime();
                    output << " prio=" << process->dynamicPriority();
                    output << '\n';
                }
                break;
            }
            case TRANS_PREEMPT: {
                assert(currentProcess == process);
                callScheduler = true;
                currentProcess = nullptr;
                if (checkTermination(process, elaspedTime, currentTime, output, verbose)) {
                    break;
                } else if (verbose) {
                    verboseHeader(output, process, currentTime, elaspedTime);
                    output << "RUNNG -> READY cb=";
                    output << process->remainingCPUBurst();
                    output << " rem=" << process->remainingCPUTime();
                    output << " prio=" << process->dynamicPriority();
                    output << '\n';
                }
                process->decrementPriority();
                d_scheduler->addProcess(process);
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
        assert(currentProcess == nullptr);
        assert(blockedProcesses == 0);
        // Calculate summary metrics
        d_finishTime = currentTime;
        double finishTime = static_cast<double>(d_finishTime);
        double processListSize = static_cast<double>(d_processList.size());
        d_cpuUsagePercentage = static_cast<double>(cpuUsageTime) / finishTime * 100;
        d_ioUsagePercentage = static_cast<double>(ioUsageTime) / finishTime * 100;
        d_processThroughput = processListSize / finishTime * 100;
        double totalWaitTime = 0;
        double totalTurnaroundTime = 0;
        for (auto process : d_processList) {
            totalWaitTime += process->waitingTime();
            totalTurnaroundTime += process->turnaroundTime();
        }
        d_averageCPUTurnaroundTime = totalTurnaroundTime / processListSize;
        d_averageCPUWaitTime = totalWaitTime / processListSize;
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
            auto process = Process::createProcess(cpuTime, cpuBurst, ioBurst, timeStamp, priority);
            process->setTransition(TRANS_READY);
            d_eventQueue.addEvent(timeStamp, process);
            d_processList.push_back(process);
        }
    }
    bool Simulation::checkTermination(Process* process, int elaspedTime, int currentTime, std::ostream& output, bool verbose)
    {
        process->runCPU(elaspedTime);
        if (process->remainingCPUTime() != 0) {
            return false;
        }
        process->setFinishTime(currentTime);
        if (verbose) {
            verboseHeader(output, process, currentTime, elaspedTime);
            output << "Done\n";
        }
        return true;
    }
    void Simulation::verboseHeader(std::ostream& output, Process* process, int currentTime, int processElaspedTime)
    {
        output << currentTime << ' ' << process->pid() << ' ' << processElaspedTime << ": ";
    }
    std::ostream& operator<<(std::ostream& out, const Simulation& simulation)
    {
        for (auto process : simulation.d_processList) {
            out << (*process);
        }
        // Again, don't like the wait C++ handles output precision and padding
        // So cumbersome!
        char output[200];
        sprintf(output, "SUM: %d %.2lf %.2lf %.2lf %.2lf %.3lf\n",
            simulation.d_finishTime, simulation.d_cpuUsagePercentage,
            simulation.d_ioUsagePercentage, simulation.d_averageCPUTurnaroundTime,
            simulation.d_averageCPUWaitTime, simulation.d_processThroughput);
        out << std::string(output) << '\n';
    }
}
}