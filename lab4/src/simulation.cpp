#include "simulation.h"
#include "scheduler.h"
#include <istream>
#include <ostream>
#include <sstream>
#include <stdio.h>
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
            ++id;
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
        unsigned int totalMovement = 0;
        while (!(d_scheduler->empty() && d_notArrived.empty() && currentRequest == nullptr)) {
            if (!d_notArrived.empty() && d_notArrived.front()->arriveTime() == currentTime) {
                d_scheduler->addToQueue(d_notArrived.front());
                d_notArrived.pop();
            }
            if (currentRequest != nullptr) {
                if (currentRequest->track() > headPosition) {
                    ++headPosition;
                    ++totalMovement;
                } else if (currentRequest->track() < headPosition) {
                    --headPosition;
                    ++totalMovement;
                }
                if (headPosition == currentRequest->track()) {
                    currentRequest->endTime() = currentTime;
                    currentRequest = nullptr;
                }
            }
            while (currentRequest == nullptr && !d_scheduler->empty()) {
                currentRequest = d_scheduler->getRequest();
                currentRequest->startTime() = currentTime;
                if (currentRequest->track() == headPosition) {
                    currentRequest->endTime() = currentTime;
                    currentRequest = nullptr;
                } else {
                    break;
                }
            }
            ++currentTime;
        }
        --currentTime;
        double totTurn = 0;
        double totWait = 0;
        unsigned int maxWait = 0;
        for (auto ptr : d_requests) {
            output << (*ptr);
            totTurn += ptr->endTime() - ptr->arriveTime();
            unsigned int waitTime = ptr->startTime() - ptr->arriveTime();
            maxWait = waitTime > maxWait ? waitTime : maxWait;
            totWait += waitTime;
        }
        double avgTurn = totTurn / d_requests.size();
        double avgWait = totWait / d_requests.size();
        char buffer[256];
        sprintf(buffer, "SUM: %d %d %.2lf %.2lf %d\n",
            currentTime, totalMovement, avgTurn, avgWait, maxWait);
        output << buffer;
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