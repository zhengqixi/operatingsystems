#include "fifohandler.h"
#include "pagehandler.h"
#include "simulation.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>
int main(int argc, char* argv[])
{
    using namespace NYU::OperatingSystems;
    char faultAlgorithm = '\0';
    bool output, pageTable, frameTable, summary, verboseCurrPageTable, verboseAllPageTable,
        verboseFrameTable, verboseAging = false;
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
    std::ifstream randomFile(argv[optind]);
    PageHandler* faultHandler = nullptr;
    switch (faultAlgorithm) {
    case 'f':
        faultHandler = new FifoHandler(numFrames);
        break;
    case 'r':
        break;
    case 'c':
        break;
    case 'e':
        break;
    case 'a':
        break;
    case 'w':
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