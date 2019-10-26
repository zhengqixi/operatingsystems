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
        static std::shared_ptr<Process> createProcess(int cpuTime, int cpuBurst, int ioBurst, long createTime, int staticPriority);
        // Get process ID
        PID pid() const;
        // Get total remaining CPU time
        int totalCPUTime() const;
        // Run the cpu for the specified amount. This reduces remaining CPU time
        void runCPU(int cpuRun);
        // Get the current timestamp for this process
        long timeStamp() const;
        // Set the update the timestamp
        void setTimeStamp(long newTime);
        // Get time process was created at
        long createTime() const;
        // Get the cpu burst
        int cpuBurst() const;
        // Get the io burst
        int ioBurst() const;
        // Get next state to transition to
        PROCESS_TRANSITIONS transition() const;
        // set next state to transition to
        void setTransition(PROCESS_TRANSITIONS next);
        // Get the dynamic priority
        // This is defined as [0...static priority - 1]
        int dynamicPriority() const;
        // Get the static priority
        int staticPriority() const;
        // Decrement the dynamic priority. Wraps if goes to -1
        void decrementPriority();
        // resets the dynamic priority to max
        void resetDynamicPriority();
        // Get the remaining amount of CPU burst left
        int remainingCpuBurst() const;
        // Sets the remaining amount of CPU burst left
        void setCpuBurst(int burst);
        Process(PID pid, int cpuTime, int cpuBurst, int ioBurst, long creationTime, int staticPriority);

    private:
        PID d_pid;
        PROCESS_TRANSITIONS d_transitionNext = TRANS_READY;
        long d_timeStamp;
        int d_totalCpuTime;
        int d_cpuBurst;
        int d_ioBurst;
        long d_creationTime;
        int d_staticPriority;
        int d_dynamicPriority;
        int d_remainingCpuBurst = 0;
    };
}
}
#endif