#include "process.h"
#include <memory>
namespace NYU {
namespace OperatingSystems {
    Process::Process(PID pid, int cpuTime, int cpuBurst, int ioBurst, long timeStamp)
        : d_pid{ pid }
        , d_totalCpuTime{ cpuTime }
        , d_cpuBurst{ cpuBurst }
        , d_ioBurst{ ioBurst }
        , d_timeStamp{ timeStamp }
    {
    }
    std::shared_ptr<Process> Process::createProcess(int cpuTime, int cpuBurst, int ioBurst, long timeStamp)
    {
        static PID newPid = 0;
        auto newProcess = std::shared_ptr<Process>(new Process(newPid, cpuTime, cpuBurst, ioBurst, timeStamp));
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
    void Process::runCPU(int cpuRun)
    {
        d_totalCpuTime -= cpuRun;
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
    long Process::timeStamp() const
    {
        return d_timeStamp;
    }
    void Process::setTimeStamp(long newTime)
    {
        d_timeStamp = newTime;
    }
}
}