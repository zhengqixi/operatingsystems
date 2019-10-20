#include "process.h"
#include <memory>
namespace NYU {
namespace OperatingSystems {
    Process::Process(PID pid, int cpuTime, int cpuBurst, int ioBurst)
        : d_pid{ pid }
        , d_totalCpuTime{ cpuTime }
        , d_cpuBurst{ cpuBurst }
        , d_ioBurst{ ioBurst }
    {
    }
    std::shared_ptr<Process> Process::createProcess(int cpuTime, int cpuBurst, int ioBurst)
    {
        static PID newPid = 0;
        auto newProcess = std::make_shared<Process>(newPid, cpuTime, cpuBurst, ioBurst);
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
}
}