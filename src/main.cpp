#include <iostream>
#include "EventsMonitor.h"
int main() {
    EventsMonitor monitor(1, true, true, false, false, false, false);
    monitor.~EventsMonitor();
    return 0;
}