#include "EventsMonitor.h"

EventsMonitor::EventsMonitor(int num_threads,
                bool cpu_monitor,
                bool memory_monitor,
                bool disk_monitor,
                bool network_monitor,
                bool process_monitor) :
                is_initialized(true),
                monitor_events{
                  cpu_monitor,
                  memory_monitor,
                  disk_monitor,
                  network_monitor,
                  process_monitor
                },
                thread_pool(num_threads)
                {}
EventsMonitor::~EventsMonitor(){
  stop();
}
void EventsMonitor::start() {
    // Intentionally left blank.
  if(!is_initialized || is_running.load()){
    return ;
  }
  is_running.store(true);
  worker = std::jthread(&EventsMonitor::run_loop,this);
}

void EventsMonitor::stop(){
  if(!is_running.exchange(false)){
    return;
  }
  if(worker.joinable()){
    worker.join();
  }
  thread_pool.stop();
}

std::string EventsMonitor::get_now_time() const {
  using namespace std::chrono;
  return std::format("{:%Y-%m-%d %H:%M:%S}",system_clock::now());
}
void EventsMonitor::run_loop(){
  if(!is_running){
    return ;
  }
  while(is_running.load()){
    using enum Monitor_type;
    std::vector<std::future<json>>results;
    if(monitor_events[(size_t)CPU]){
      results.emplace_back(thread_pool.enqueue(cpuMonitor{}));
    }
    if(monitor_events[(size_t)DISK]){
      results.emplace_back(thread_pool.enqueue(DiskMonitor{}));
    }
    if(monitor_events[(size_t)MEMORY]){
      results.emplace_back(thread_pool.enqueue(MemoryMonitor{}));
    }
    if(monitor_events[(size_t)NETWORK]){
      results.emplace_back(thread_pool.enqueue(NetworkMonitor{}));
    }
    if(monitor_events[(size_t)PROCESS]){
      results.emplace_back(thread_pool.enqueue(ProcessMonitor{}));
    }
    std::ofstream out("log.txt",std::ios::app | std::ios::out);
    out << "Current Time:"  << get_now_time() << std::endl;
    for(auto& json_results : results){
      out << json_results.get().dump(4)<<std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}