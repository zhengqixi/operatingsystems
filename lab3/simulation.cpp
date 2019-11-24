#include "simulation.h"
#include "page.h"
#include "pagehandler.h"
#include "process.h"
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    Simulation::Simulation(std::istream& input, PageHandler* faultHandler, int numPages)
        : d_faultHandler{ faultHandler }
        , d_input{ input }
    {
        // Create process list
        // No need to worry about bad inputs :)
        int processCount = std::stoi(nextLine());
        d_processList.reserve(processCount);
        for (int i = 0; i < processCount; ++i) {
            d_processList.push_back(Process(numPages));
            int vmaCount = std::stoi(nextLine());
            for (int j = 0; j < vmaCount; ++j) {
                std::stringstream lineParser(nextLine());
                int start, end, writeProtected, fileMapped;
                lineParser >> start >> end >> writeProtected >> fileMapped;
                d_processList[i].addVMA(start, end,
                    static_cast<bool>(fileMapped),
                    static_cast<bool>(writeProtected));
            }
        }
    }
    void Simulation::run(std::ostream& output, bool perInstOutput)
    {
        unsigned long currentInst = 0;
        int currentProcess = -1;
        char inst = '\0';
        int operand = -1;
        while (nextInstruction(inst, operand)) {
            if (perInstOutput) {
                output << currentInst << ": ==> " << inst
                       << ' ' << operand << '\n';
            }
            if (inst == 'c') {
                currentProcess = operand;
                continue;
            }
            if (inst == 'e') {
                processExist(currentProcess);
                currentProcess = -1;
                continue;
            }
            auto& page = d_processList[currentProcess].getPage(operand);
            bool noSegFault = true;
            if (!page.present()) {
                if (!d_processList[currentProcess].setPageBits(operand)) {
                    noSegFault = false;
                    // SIGSEGV!
                } else {
                    // Handle fault
                }
            }
            if (noSegFault) {
                // Continue with rest of stuff
            }
            ++currentInst;
        }
    }
    void Simulation::processExist(int processNum)
    {
        auto& pageTable = d_processList[processNum].pageTable();
        for (auto& pte : pageTable) {
            if (pte.present()) {
                // Remove from global table
                d_faultHandler->freeFrame(pte.assignedFrame());
            }
        }
    }
    bool Simulation::nextInstruction(char& inst, int& operand)
    {
        std::string instruction(nextLine());
        if (instruction.empty()) {
            return false;
        }
        std::stringstream instParser(instruction);
        instParser >> inst >> operand;
        return true;
    }
    std::string Simulation::nextLine()
    {
        std::string line;
        while (std::getline(d_input, line)) {
            if (line.front() == '#') {
                continue;
            }
            return line;
        }
        return "";
    }
}
}