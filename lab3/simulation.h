#ifndef SIMULATION_H
#define SIMULATION_H
#include "page.h"
#include "pagehandler.h"
#include "process.h"
#include <istream>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class Simulation {
    public:
        Simulation(std::istream& input, PageHandler* faultHandler, int numPages);
        void run(std::ostream& output, bool perInstOutput, bool finalFrameTable, bool finalPageTable, bool processSummary, bool summary);

    private:
        PageHandler* d_faultHandler = nullptr;
        bool nextInstruction(char& inst, int& operand);
        std::string nextLine();
        std::pair<int, int> processExist(int processNum, std::ostream& output, bool unmapOut);
        void printFrametable(std::ostream& output);
        void printProcessPageTable(int processNum, std::ostream& output);
        void printAllPageTables(std::ostream& output);
        void printProcessSummary(int processNum, std::ostream& output);
        void printAllProcessSummary(std::ostream& output);
        std::istream& d_input;
        std::vector<Process> d_processList;
        // Cycle cost
        unsigned long d_maps = 400;
        unsigned long d_pageInOut = 3000;
        unsigned long d_fileInOut = 2500;
        unsigned long d_zero = 150;
        unsigned long d_segV = 240;
        unsigned long d_segProt = 300;
        unsigned long d_access = 1;
        unsigned long d_contextSwitch = 121;
        unsigned long d_processExist = 175;
    };
}
}
#endif