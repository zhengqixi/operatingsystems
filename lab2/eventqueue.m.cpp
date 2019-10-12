#include "event.h"
#include "eventqueue.h"
#include <iostream>
int main()
{
    using namespace NYU::OperatingSystems;
    EventQueue<int> queue;
    queue.AddEvent(4, 4);
    queue.AddEvent(4, 5);
    queue.AddEvent(9, 9);
    queue.AddEvent(10, 10);
    queue.AddEvent(6, 6);
    queue.AddEvent(7, 7);
    queue.AddEvent(1, 1);
    queue.AddEvent(1, 2);
    queue.AddEvent(3, 3);
    queue.AddEvent(8, 8);
    while (queue.Continue()) {
        auto event = queue.PopEvent();
        std::cout << "Timestamp: " << event.TimeStamp();
        std::cout << " Data: " << event.Data() << '\n';
    }
    return 0;
}