#ifndef PROCESS_H
#define PROCESS_H
namespace NYU {
namespace OperatingSystems {
    enum PROCESS_STATES {
        CREATED,
        READY,
        RUNNING,
        BLOCKED
    };
    typedef unsigned int PID;
    class Process {
    public:
        Process(PID pid, int cpuTime, int cpuBurst, int ioBurst);
        // Get process ID
        PID pid() const;
        // Get total remaining CPU time
        int totalCPUTime() const;
        // Run the cpu for the specified amount. This reduces remaining CPU time
        void runCPU(int cpuRun);
        // Get the cpu burst
        int cpuBurst() const;
        // Get the io burst
        int ioBurst() const;
        // Get the current state
        PROCESS_STATES getState() const;
        // Get the next state to transition to
        PROCESS_STATES nextState() const;
        // Sets the next state to transition to
        void setNextState(PROCESS_STATES nextState);
        // Sets the current state
        void setState(PROCESS_STATES newState);

    private:
        PID d_pid;
        PROCESS_STATES d_currState = CREATED;
        PROCESS_STATES d_nextState = READY;
        int d_totalCpuTime;
        int d_cpuBurst;
        int d_ioBurst;
    };
}
}
#endif