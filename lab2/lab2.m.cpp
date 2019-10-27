#include "abstractscheduler.h"
#include "fifoscheduler.h"
#include "lifoscheduler.h"
#include "simulation.h"
#include "srtfscheduler.h"
#include <fstream>
#include <iostream>
#include <unistd.h>

int main(int argc, char* argv[])
{
    using namespace NYU::OperatingSystems;
    // Data to be set by argument parser
    int opt = -1;
    AbstractScheduler* scheduler = nullptr;
    std::string schedulerName;
    std::string inputFileName;
    std::string randomFileName;
    bool verbose = false;
    int quantum = 10000;
    int maxPriority = 4;
    // Not gonna support e and t options because I frankly have no idea what they mean
    while ((opt = getopt(argc, argv, "vs:")) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;
        case 's':
            switch (optarg[0]) {
            case 'F':
                scheduler = new FIFOScheduler();
                schedulerName = "FCFS";
                break;
            case 'L':
                scheduler = new LIFOScheduler();
                schedulerName = "LCFS";
                break;
            case 'S':
                scheduler = new SRTFScheduler();
                schedulerName = "SRTF";
                break;
            case 'R':
                sscanf(optarg, "R%d", &quantum);
                scheduler = new FIFOScheduler(quantum);
                schedulerName = "RR " + std::to_string(quantum);
                break;
            default:
                std::cerr << "Unsupported scheduler\n";
                return -1;
            }
            break;
        default:
            return -1;
        }
    }
    if (argc - optind < 2) {
        std::cerr << "Not enough arguments\n";
        return -1;
    }
    if (scheduler == nullptr) {
        std::cerr << "No scheduler set. Existing\n";
        return -1;
    }
    inputFileName = argv[optind];
    ++optind;
    randomFileName = argv[optind];
    // Parse arguments;
    // Simulate
    std::ifstream input(inputFileName);
    std::ifstream random(randomFileName);
    Simulation simulation(input, random, scheduler);
    simulation.Simulate(std::cout, verbose);
    std::cout << schedulerName << '\n';
    std::cout << simulation;
    return 0;
}
