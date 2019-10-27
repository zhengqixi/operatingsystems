#include "process.h"
#include <memory>
#include <ostream>
#include <stdio.h>
#include <string>
namespace NYU {
namespace OperatingSystems {
    Process::Process(PID pid, int cpuTime, int cpuBurst, int ioBurst, int creationTime, int staticPriority)
        : d_pid{ pid }
        , d_totalCpuTime{ cpuTime }
        , d_remainingCpuTime{ cpuTime }
        , d_cpuBurst{ cpuBurst }
        , d_ioBurst{ ioBurst }
        , d_timeStamp{ creationTime }
        , d_creationTime{ creationTime }
        , d_staticPriority{ staticPriority }
        , d_dynamicPriority{ staticPriority - 1 }
    {
    }
    std::shared_ptr<Process> Process::createProcess(int cpuTime, int cpuBurst, int ioBurst, int creationTime, int staticPriority)
    {
        static PID newPid = 0;
        auto newProcess = std::make_shared<Process>(Process(newPid, cpuTime, cpuBurst, ioBurst, creationTime, staticPriority));
        ++newPid;
        return newProcess;
    }
    PID Process::pid() const
    {
        return d_pid;
    }
    int Process::totalCPUTime() const
    {
        return d_totalCpuTime;
    }
    int Process::remainingCPUTime() const
    {
        return d_remainingCpuTime;
    }
    int Process::createTime() const
    {
        return d_creationTime;
    }
    void Process::runCPU(int cpuRun)
    {
        d_remainingCpuTime -= cpuRun;
        d_remainingCpuBurst -= cpuRun;
    }
    int Process::cpuBurst() const
    {
        return d_cpuBurst;
    }
    int Process::ioBurst() const
    {
        return d_ioBurst;
    }
    PROCESS_TRANSITIONS Process::transition() const
    {
        return d_transitionNext;
    }
    // set next state to transition to
    void Process::setTransition(PROCESS_TRANSITIONS next)
    {
        d_transitionNext = next;
    }
    int Process::timeStamp() const
    {
        return d_timeStamp;
    }
    void Process::setTimeStamp(int newTime)
    {
        d_timeStamp = newTime;
    }
    int Process::dynamicPriority() const
    {
        return d_dynamicPriority;
    }
    int Process::staticPriority() const
    {
        return d_staticPriority;
    }
    void Process::decrementPriority()
    {
        if (d_dynamicPriority == 0) {
            d_dynamicPriority = d_staticPriority - 1;
        } else {
            --d_dynamicPriority;
        }
    }
    void Process::resetDynamicPriority()
    {
        d_dynamicPriority = d_staticPriority - 1;
    }
    int Process::remainingCPUBurst() const
    {
        return d_remainingCpuBurst;
    }
    void Process::setCPUBurst(int burst)
    {
        d_remainingCpuBurst = burst;
    }
    void Process::addIOTime(int IOTime)
    {
        d_ioTime += IOTime;
    }
    void Process::addReadyTime(int readyTime)
    {
        d_readyTime += readyTime;
    }
    void Process::setFinishTime(int finishTime)
    {
        d_finishTime = finishTime;
    }
    std::ostream& operator<<(std::ostream& out, const Process& process)
    {
        // Really don't feel like doing the C++ way of printing the padding...
        // A lot of overhead for something that should be so simple. ugh..
        int turnAroundTime = process.d_finishTime - process.d_creationTime;
        char line[100];
        sprintf(line, "%04d: %4d %4d %4d %4d %1d | %5d %5d %5d %5d\n",
            process.d_pid, (int)process.d_creationTime, process.d_totalCpuTime,
            process.d_cpuBurst, process.d_ioBurst, process.d_staticPriority,
            (int)process.d_finishTime, turnAroundTime, process.d_ioTime, process.d_readyTime);
        out << std::string(line);
        return out;
    }
}
}