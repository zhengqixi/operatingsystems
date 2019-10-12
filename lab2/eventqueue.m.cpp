#include "event.h"
#include "eventqueue.h"
#include <iostream>
int main()
{
    using namespace NYU::OperatingSystems;
    EventQueue<int> queue;
    queue.addEvent(4, 4);
    queue.addEvent(4, 5);
    queue.addEvent(9, 9);
    queue.addEvent(10, 10);
    queue.addEvent(6, 6);
    queue.addEvent(7, 7);
    queue.addEvent(1, 1);
    queue.addEvent(1, 2);
    queue.addEvent(3, 3);
    queue.addEvent(8, 8);
    while (queue.hasEvents()) {
        auto event = queue.popEvent();
        std::cout << "Timestamp: " << event.timeStamp();
        std::cout << " Data: " << event.data() << '\n';
    }
    return 0;
}