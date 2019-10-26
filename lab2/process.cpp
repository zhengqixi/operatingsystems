#include "process.h"
#include <memory>
namespace NYU {
namespace OperatingSystems {
    Process::Process(PID pid, int cpuTime, int cpuBurst, int ioBurst, long creationTime, int staticPriority)
        : d_pid{ pid }
        , d_totalCpuTime{ cpuTime }
        , d_cpuBurst{ cpuBurst }
        , d_ioBurst{ ioBurst }
        , d_timeStamp{ creationTime }
        , d_creationTime{ creationTime }
        , d_staticPriority{ staticPriority }
        , d_dynamicPriority{ staticPriority - 1 }
    {
    }
    std::shared_ptr<Process> Process::createProcess(int cpuTime, int cpuBurst, int ioBurst, long creationTime, int staticPriority)
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
    long Process::createTime() const
    {
        return d_creationTime;
    }
    void Process::runCPU(int cpuRun)
    {
        d_totalCpuTime -= cpuRun;
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
    long Process::timeStamp() const
    {
        return d_timeStamp;
    }
    void Process::setTimeStamp(long newTime)
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
    int Process::remainingCpuBurst() const
    {
        return d_remainingCpuBurst;
    }
    void Process::setCpuBurst(int burst)
    {
        d_remainingCpuBurst = burst;
    }
}
}