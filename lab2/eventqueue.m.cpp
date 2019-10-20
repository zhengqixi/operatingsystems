#include "eventqueue.h"
#include <iostream>
int main()
{
    using namespace NYU::OperatingSystems;
    EventQueue<int> queue;
    queue.addEvent(4, 4);
    queue.addEvent(4, 5);
    queue.addEvent(8, 8);
    queue.addEvent(6, 6);
    queue.addEvent(7, 7);
    queue.addEvent(1, 1);
    queue.addEvent(1, 232123); // should not be removed
    queue.addEvent(7, 12345); // should be removed
    queue.addEvent(1, 2);
    queue.addEvent(3, 3);
    queue.addEvent(12, 11232); // should be removed
    for (int i = 0; i < 8; ++i) {
        auto event = queue.popEvent();
        queue.removeEvent(11232, event.timeStamp());
        queue.removeEvent(232123, event.timeStamp());
        queue.removeEvent(12345, event.timeStamp());
        std::cout << "Timestamp: " << event.timeStamp();
        std::cout << " Data: " << event.data();
        std::cout << " Next time stamp: " << queue.peekNextTimeStamp() << '\n';
    }
    queue.addEvent(10, 10);
    queue.addEvent(8, 9);
    auto event = queue.popEvent();
    std::cout << "Timestamp: " << event.timeStamp();
    std::cout << " Data: " << event.data();
    std::cout << " Next time stamp: " << queue.peekNextTimeStamp() << '\n';
    event = queue.popEvent();
    std::cout << "Timestamp: " << event.timeStamp();
    std::cout << " Data: " << event.data();
    std::cout << " Next time stamp: " << queue.peekNextTimeStamp() << '\n';
    while (queue.peekNextTimeStamp() != -1) {
        event = queue.popEvent();
        std::cout << "Timestamp: " << event.timeStamp();
        std::cout << " Data: " << event.data();
        std::cout << " Next time stamp: " << queue.peekNextTimeStamp() << '\n';
    }
    return 0;
}