#ifndef SIMULATION_H
#define SIMULATION_H
#include "page.h"
#include "pagehandler.h"
#include "process.h"
#include <istream>
#include <ostream>
#include <string>
#include <vector>
namespace NYU {
namespace OperatingSystems {
    class Simulation {
    public:
        Simulation(std::istream& input, PageHandler* faultHandler, int numPages);
        void run(std::ostream& output);

    private:
        PageHandler* d_faultHandler = nullptr;
        bool nextInstruction(char& inst, int& operand);
        std::string nextLine();
        std::istream& d_input;
        std::vector<Process> d_processList;
    };
}
}
#endif