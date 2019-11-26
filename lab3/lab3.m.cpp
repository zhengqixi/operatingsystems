#include "agehandler.h"
#include "clockhandler.h"
#include "fifohandler.h"
#include "nruhandler.h"
#include "pagehandler.h"
#include "randomhandler.h"
#include "simulation.h"
#include "workingsethandler.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
int main(int argc, char* argv[])
{
    using namespace NYU::OperatingSystems;
    char faultAlgorithm = '\0';
    bool output, pageTable, frameTable, summary, verboseCurrPageTable, verboseAllPageTable,
        verboseFrameTable, verboseFault = false;
    int opt = -1;
    int numFrames = 128;
    int numPages = 64;
    while ((opt = getopt(argc, argv, "a:o:f:")) != -1) {
        switch (opt) {
        case 'a':
            faultAlgorithm = optarg[0];
            break;
        case 'o': {
            std::string options(optarg);
            for (char c : options) {
                switch (c) {
                case 'O':
                    output = true;
                    break;
                case 'P':
                    pageTable = true;
                    break;
                case 'F':
                    frameTable = true;
                    break;
                case 'S':
                    summary = true;
                    break;
                case 'a':
                    verboseFault = true;
                    break;
                }
            }
            break;
        }

        case 'f':
            numFrames = std::stoi(optarg);
            break;
        default:
            break;
        }
    }
    if (argc - optind < 2) {
        std::cerr << "Not enough arguments\n";
        return -1;
    }
    if (faultAlgorithm == '\0') {
        std::cerr << "No page fault algorithm set. Existing\n";
        return -1;
    }
    std::ifstream input(argv[optind]);
    ++optind;
    PageHandler* faultHandler = nullptr;
    switch (faultAlgorithm) {
    case 'f':
        faultHandler = new FifoHandler(numFrames);
        break;
    case 'r':
        faultHandler = new RandomHandler(numFrames, argv[optind]);
        break;
    case 'c':
        faultHandler = new ClockHandler(numFrames);
        break;
    case 'e':
        faultHandler = new NRUHandler(numFrames, 50, verboseFault, std::cout);
        break;
    case 'a':
        faultHandler = new AgeHandler(numFrames, verboseFault, std::cout);
        break;
    case 'w':
        faultHandler = new WorkingSetHandler(numFrames, 50, verboseFault, std::cout);
        break;
    default:
        std::cerr << "Not a valid algorithm:" << faultAlgorithm << '\n';
        return -1;
    }
    Simulation simulator(input, faultHandler, numPages);
    simulator.run(std::cout, output, frameTable, pageTable, summary);
    // Determine algorithm
    return 0;
}