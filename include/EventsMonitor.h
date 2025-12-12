#pragma once
#include <iostream>
#include <vector>
#include <future> 
#include <array>
#include <nlohmann/json.hpp>

#include "ThreadPool.h"
#include "cpuMonitor.h"
#include "memoryMonitor.h"
#include "diskMonitor.h"
#include "networkMonitor.h"
#include "processMonitor.h"
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
                bool network_monitor,bool process_monitor):
                thread_pool(num_threads),is_initialized(false),is_running(false){
    monitored_events[U::CPU_MONITOR] = cpu_monitor;
    monitored_events[U::MEMORY_MONITOR] = memory_monitor;
    monitored_events[U::DISK_MONITOR] = disk_monitor;
    monitored_events[U::NETWORK_MONITOR] = network_monitor;
    monitored_events[U::PROCESS_MONITOR] = process_monitor;
    if(num_threads <= 0){
      num_threads = 1;
      std::cout << "Number of threads cannot be less than or equal to 0. Setting number of threads to 1." << std::endl;
    }
    is_initialized = true;
    is_running = true;
    while(is_running){

      std::vector<std::future<json>> results;

      if(monitored_events[U::CPU_MONITOR]){
        cpuMonitor cpu_monitor;
        results.push_back(thread_pool.enqueue(cpu_monitor));
        
      }
      if(monitored_events[U::MEMORY_MONITOR]){
        MemoryMonitor memory_monitor;
        results.push_back(thread_pool.enqueue(memory_monitor));
      }
      if(monitored_events[U::DISK_MONITOR]){
        DiskMonitor disk_monitor;
        results.push_back(thread_pool.enqueue(disk_monitor));
      }
      if(monitored_events[U::NETWORK_MONITOR]){
        NetworkMonitor network_monitor;
        results.push_back(thread_pool.enqueue(network_monitor));
      }
      if(monitored_events[U::PROCESS_MONITOR]){
        ProcessMonitor process_monitor;
        results.push_back(thread_pool.enqueue(process_monitor));
      }
      std::ofstream out ("log.txt" , std::ofstream::out | std::ofstream::app);
      //写入时间戳
      
      time_t now = time(0);
      char* dt = ctime(&now);
      out << "Current Time: " << dt << std::endl;
      for(auto & result : results){ 
        json j = result.get();
        out << j.dump(4) << std::endl;
      }
      out.close();

      sleep(10);
    }

  }
private:
  bool is_initialized;
  bool is_running;
  std::array<bool,6> monitored_events;

  ThreadPool thread_pool;
};
