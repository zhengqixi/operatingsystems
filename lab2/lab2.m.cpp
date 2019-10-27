#include "abstractscheduler.h"
#include "fifoscheduler.h"
#include "simulation.h"
#include <fstream>
#include <iostream>
#include <memory>
int main(int argc, char* argv[])
{
    using namespace NYU::OperatingSystems;
    std::shared_ptr<AbstractScheduler> scheduler = nullptr;
    std::ifstream input(argv[1]);
    std::ifstream random(argv[2]);
    scheduler = std::static_pointer_cast<AbstractScheduler>(std::make_shared<FifoScheduler>());
    Simulation simulation(input, random, scheduler);
    simulation.Simulate(std::cout, true);
    std::cout << simulation;
    // Process arguments, create input streams
    // Determine right scheduler to create and pass through
    return 0;
}