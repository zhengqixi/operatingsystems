#ifndef PROCESS_H
#define PROCESS_H
#include <ostream>
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
        // Constructor
        Process(PID pid, int cpuTime, int cpuBurst, int ioBurst, int creationTime, int staticPriority);
        // Static method for creating processes
        static Process* createProcess(int cpuTime, int cpuBurst, int ioBurst, int createTime, int staticPriority);
        // Get process ID
        PID pid() const;
        // Get total CPU time
        int totalCPUTime() const;
        // Get the remaining CPU time
        int remainingCPUTime() const;
        // Run the cpu for the specified amount. This reduces remaining CPU time remaining CPU burst time
        void runCPU(int cpuRun);
        // Get the current timestamp for this process
        int timeStamp() const;
        // Set the update the timestamp
        void setTimeStamp(int newTime);
        // Get time process was created at
        int createTime() const;
        // Get the cpu burst characterization parameter
        int cpuBurst() const;
        // Get the remaining amount of CPU burst left
        int remainingCPUBurst() const;
        // Sets the amount of CPU burst to run
        void setCPUBurst(int burst);
        // Get the io burst characterization parameter
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
        // Book keeping functions
        // Increases amount of time spent waiting on IO
        void addBlockedTime(int IOTime);
        // Increases the amount of time spent in ready queue
        void addWaitingTime(int readyTime);
        // Set the finish time
        void setFinishTime(int finishTime);
        // Get the waiting time spent in ready
        int waitingTime() const;
        // Get turnaround time
        int turnaroundTime() const;
        friend std::ostream& operator<<(std::ostream& out, const Process& process);

    private:
        // Characterization parameters
        PID d_pid;
        int d_creationTime;
        int d_totalCpuTime;
        int d_cpuBurst;
        int d_ioBurst;
        int d_staticPriority;
        // Simulation parameters
        PROCESS_TRANSITIONS d_transitionNext = TRANS_READY;
        int d_timeStamp;
        int d_remainingCpuTime;
        int d_dynamicPriority;
        int d_remainingCpuBurst = 0;

        // Book keeping parameters

        // Time waiting for IO
        int d_blockedTime = 0;
        // Time waiting in run queue
        int d_waitingTime = 0;
        int d_finishTime = 0;
        int d_turnAroundTime = 0;
    };
}
}
#endif