#include "process.h"
namespace NYU {
namespace OperatingSystems {
    Process::Process(PID pid, int cpuTime, int cpuBurst, int ioBurst)
        : d_pid(pid)
        , d_totalCpuTime(cpuTime)
        , d_cpuBurst(cpuBurst)
        , d_ioBurst(ioBurst)
    {
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
    PROCESS_STATES Process::getState() const
    {
        return d_currState;
    }
    PROCESS_STATES Process::nextState() const
    {
        return d_nextState;
    }
    void Process::setNextState(PROCESS_STATES nextState)
    {
        d_nextState = nextState;
    }
    // Sets the current state
    void Process::setState(PROCESS_STATES newState)
    {
        d_currState = newState;
    }
}
}