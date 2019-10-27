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
    Simulation::~Simulation()
    {
        for (auto process : d_processList) {
            delete process;
        }
    }
    void Simulation::Simulate(std::ostream& output, bool verbose)
    {
        int quantum = d_scheduler->quantum();
        bool callScheduler = false;
        int currentTime = 0;
        // If a process transitioning to ready preempts the current process,
        // This is set to guard against multiple processes preempting the current process
        // One the scheduler is called, the new process is run and this is no inter true
        bool processBootEvent = false;
        Process* currentProcess = nullptr;
        while (d_eventQueue.hasEvents()) {
            auto event = d_eventQueue.popEvent();
            currentTime = event.timeStamp();
            auto process = event.data();
            int elaspedTime = currentTime - process->timeStamp();
            assert(elaspedTime >= 0);
            process->setTimeStamp(currentTime);
            switch (process->transition()) {
            case TRANS_READY: {
                callScheduler = true;
                if (process->createTime() != currentTime) {
                    if (verbose) {
                        verboseHeader(output, process, currentTime);
                        output << "BLOCK -> READY\n";
                    }
                    process->addIOTime(elaspedTime);
                } else if (verbose) {
                    verboseHeader(output, process, currentTime);
                    output << "CREATED -> READY\n";
                }
                d_scheduler->addProcess(process);
                process->resetDynamicPriority();
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
                    verboseHeader(output, process, currentTime);
                    output << "RUNNG -> BLOCK ib=";
                    output << ioBurst << " rem=";
                    output << process->remainingCPUTime();
                    output << '\n';
                }
                break;
            }
            case TRANS_RUN: {
                // Current process must be taken out by a preemption event or something first
                assert(currentProcess == nullptr);
                process->addReadyTime(elaspedTime);
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
                    verboseHeader(output, process, currentTime);
                    output << "READY -> RUNNG cb=";
                    output << runTime << " rem=";
                    output << process->remainingCPUTime();
                    output << " prio=" << process->staticPriority();
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
                }
                d_scheduler->addProcess(process);
                process->decrementPriority();
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
            verboseHeader(output, process, currentTime);
            output << "DONE\n";
        }
        return true;
    }
    void Simulation::verboseHeader(std::ostream& output, Process* process, int currentTime)
    {
        output << currentTime << ' ' << process->pid() << ": ";
    }
    std::ostream& operator<<(std::ostream& out, const Simulation& simulation)
    {
        for (auto process : simulation.d_processList) {
            out << (*process);
        }
    }
}
}