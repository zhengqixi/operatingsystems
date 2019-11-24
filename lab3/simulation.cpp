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
    void Simulation::run(std::ostream& output)
    {
        int currentProcess = -1;
        char inst = '\0';
        int operand = -1;
        while (nextInstruction(inst, operand)) {
            if (inst == 'c') {
                currentProcess = operand;
                continue;
            }
            if (inst == 'e') {
                currentProcess = -1;
                continue;
            }
            auto& page = d_processList[currentProcess].getPage(operand);
            if (!page.present()) {
                // Handle fault
            }
            // Continue with rest of stuff
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