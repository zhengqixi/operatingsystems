#include "fifo.h"
#include "scheduler.h"
#include "simulation.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
int main(int argc, char* argv[])
{
    using namespace NYU::OperatingSystems;
    Scheduler* scheduler = nullptr;
    int opt = -1;
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
        case 's':
            switch (optarg[0]) {
            case 'i':
                scheduler = new Fifo();
                break;
            case 'j':
                break;
            case 's':
                break;
            case 'c':
                break;
            case 'f':
                break;
            default:
                std::cerr << "Unkown scheduler\n";
                return -1;
            }
            break;
        }
    }
    if (scheduler == nullptr) {
        std::cerr << "No scheduler. Exiting\n";
        return -1;
    }
    if ((argc - optind) < 1) {
        std::cerr << "Missing inputs. Existing\n";
        return -1;
    }
    std::ifstream input(argv[optind]);
    Simulation sim(input, scheduler);
    sim.run(std::cout);
}