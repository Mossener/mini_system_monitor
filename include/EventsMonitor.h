#pragma once
#include <iostream>
#include <vector>
#include <future> 
#include <nlohmann/json.hpp>
#include "ThreadPool.h"
#include "cpuMonitor.h"

using json = nlohmann::json;
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
  ~EventsMonitor(){
    if(is_running){
      is_running = false;
      thread_pool.stop();
    }
  }
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
    is_running = true;
    for(int i =0;i<6;i++){
      if(monitored_events[i]){
        switch(i){
          case U::CPU_MONITOR:
            break;
        }
      }
    }
    while(is_running){
      sleep(1);
      for(int i = 0;i<num_threads;i++){
        auto future = thread_pool.enqueue([]() -> json {
          cpuMonitor monitor;
          return json(monitor);
        });
        json result = future.get();
        std::cout<< result << std::endl;
      }
    }
  }
private:
  bool is_initialized;
  bool is_running;
  bool monitored_events[6];

  ThreadPool thread_pool;
};
