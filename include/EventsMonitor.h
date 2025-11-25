#pragma once
#include <iostream>
#include <vector>
#include <future> 
#include "ThreadPool.h"

enum  U:int{
  CPU_MONITOR = 0,
  MEMORY_MONITOR = 1,
  DISK_MONITOR = 2,
  NETWORK_MONITOR = 3,
  PROCESS_MONITOR = 4,
  SYSTEM_MONITOR = 5
};
class EventsMonitor {
public:
  EventsMonitor() =default;
  ~EventsMonitor();
  EventsMonitor(int num_threads,bool cpu_monitor,bool memory_monitor,bool disk_monitor,
                bool network_monitor,bool process_monitor,bool system_monitor):
                thread_pool(num_threads),is_initialized(false),is_running(false){
    monitored_events[U::CPU_MONITOR] = cpu_monitor;
    monitored_events[U::MEMORY_MONITOR] = memory_monitor;
    monitored_events[U::DISK_MONITOR] = disk_monitor;
    monitored_events[U::NETWORK_MONITOR] = network_monitor;
    monitored_events[U::PROCESS_MONITOR] = process_monitor;
    monitored_events[U::SYSTEM_MONITOR] = system_monitor;
    if(num_threads <= 0){
      memset(monitored_events,false,sizeof(monitored_events));
      std::cout << "Number of threads cannot be less than or equal to 0. Setting all monitors to false." << std::endl;
    }
    is_initialized = true;
    while(is_running){
      for(int i = 0;i<num_threads;i++){
        tasks[i]();
      }
    }
  }
private:
  bool is_initialized;
  bool is_running;
  bool monitored_events[6];
  std::vector<std::packaged_task<void()>> tasks;
  ThreadPool thread_pool;
};
