#include <iostream>
#include <string>
#include "EventsMonitor.h"
int main() {
  EventsMonitor monitor(1, true, true,false,true,true);
  monitor.start();

  return 0;
}