#include <iostream>
#include "EventsMonitor.h"
int main() {
    EventsMonitor monitor(1, true, false, false, false, false, false);

    while(true) {
        // Main thread can perform other tasks or just sleep
        sleep(10);
    }
    monitor.~EventsMonitor();
    return 0;
}