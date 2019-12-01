#include "simulation.h"
#include "scheduler.h"
#include <istream>
#include <ostream>
#include <sstream>
#include <string>
namespace NYU {
namespace OperatingSystems {
    Simulation::Simulation(std::istream& input, Scheduler* scheduler)
        : d_scheduler(scheduler)
    {
        unsigned int id = 0;
        while (true) {
            auto line = nextLine(input);
            if (line.empty()) {
                break;
            }
            std::stringstream stream(line);
            unsigned int arrival, track;
            stream >> arrival >> track;
            Request* request = new Request(id, arrival, track);
            d_requests.push_back(request);
            d_notArrived.push(request);
        }
    }
    Simulation::~Simulation()
    {
        for (auto ptr : d_requests) {
            delete ptr;
        }
        delete d_scheduler;
    }
    void Simulation::run(std::ostream& output)
    {
        Request* currentRequest = nullptr;
        // start time of current request
        unsigned int currentTime = 0;
        unsigned int headPosition = 0;
        while (!d_scheduler->empty() && !d_notArrived.empty() && currentRequest != nullptr) {
            if (d_notArrived.front()->arriveTime() == currentTime) {
                d_scheduler->addToQueue(d_notArrived.front());
                d_notArrived.pop();
            }
            if (currentRequest != nullptr && (headPosition == currentRequest->track())) {
                currentRequest->endTime() = currentTime;
                currentRequest = nullptr;
            } else if (currentRequest != nullptr) {
                if (currentRequest->track() > headPosition) {
                    ++headPosition;
                } else {
                    --headPosition;
                }
            }
            if (currentRequest == nullptr && !d_scheduler->empty()) {
                currentRequest = d_scheduler->getRequest(headPosition);
                currentRequest->startTime() = currentTime;
            }
            ++currentTime;
        }
    }

    std::string Simulation::nextLine(std::istream& input) const
    {
        std::string line;
        while (std::getline(input, line)) {
            if (line.front() == '#') {
                continue;
            }
            return line;
        }
        return "";
    }
}
}