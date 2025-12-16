#pragma once

#include <array>
#include <atomic>
#include <future>
#include <fstream>
#include <vector>
#include <chrono>
#include <thread>
#include <format>
#include <nlohmann/json.hpp>

#include "ThreadPool.h"
#include "cpuMonitor.h"
#include "memoryMonitor.h"
#include "diskMonitor.h"
#include "networkMonitor.h"
#include "processMonitor.h"

using json = nlohmann::json;
enum class Monitor_type:size_t{
  CPU = 0,
  MEMORY = 1,
  DISK = 2,
  NETWORK = 3,
  PROCESS = 4,
  SYSTEM = 5,
  COUNT = 6
};
class EventsMonitor {
public:
  EventsMonitor() =default;
  ~EventsMonitor();
  EventsMonitor(int num_threads,
                bool cpu_monitor,
                bool memory_monitor,
                bool disk_monitor,
                bool network_monitor,
                bool process_monitor);
  void start() ;
  void stop();
  void run_loop();
private:
  bool is_initialized{false};
  std::atomic_bool is_running{false};
  std::array<bool,static_cast<size_t>(Monitor_type::COUNT)>monitor_events;

  ThreadPool thread_pool;
  
  std::string get_now_time() const ;
  std::jthread worker;
};
