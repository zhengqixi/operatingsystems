#include "process.h"
#include <iostream>
int main()
{
    using namespace NYU::OperatingSystems;
    for (int i = 0; i < 100; ++i) {
        auto newProcess = Process::createProcess(100, 100, 100);
        std::cout << "new process id: " << newProcess->pid() << '\n';
    }
    return 0;
}