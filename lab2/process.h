#ifndef PROCESS_H
#define PROCESS_H
#include <memory>
namespace NYU {
namespace OperatingSystems {
    enum PROCESS_TRANSITIONS {
        TRANS_READY,
        TRANS_RUN,
        TRANS_BLOCK,
        TRANS_PREEMPT,
    };
    typedef unsigned int PID;
    class Process {
    public:
        // Static method for creating processes
        static std::shared_ptr<Process> createProcess(int cpuTime, int cpuBurst, int ioBurst);
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
        // Get next state to transition to
        PROCESS_TRANSITIONS transition() const;
        // set next state to transition to
        void setTransition(PROCESS_TRANSITIONS next);
        Process(PID pid, int cpuTime, int cpuBurst, int ioBurst);

    private:
        PID d_pid;
        PROCESS_TRANSITIONS d_transitionNext = TRANS_READY;
        int d_totalCpuTime;
        int d_cpuBurst;
        int d_ioBurst;
    };
}
}
#endif