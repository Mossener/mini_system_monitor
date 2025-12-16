#pragma once 
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
class MemoryMonitor {
public:
  json operator()(){
    json j;
    json j_data;
    j["type"] = "memory";
    j_data["total_memory"] = total_memory;
    j_data["free_memory"] = free_memory;
    j_data["used_memory"] = used_memory;
    j_data["memory_usage"] = memory_usage;
    j["data"] = j_data;
    return j;
  }
  MemoryMonitor() {
    total_memory = getTotalMemory();
    free_memory = getFreeMemory();
    used_memory = total_memory - free_memory;
    memory_usage = (used_memory * 100) / total_memory;
  }
  int getTotalMemory() const {
    std::ifstream mem_info("/proc/meminfo");
    std::string line;
    int total = 0;
    while (std::getline(mem_info, line)) {
      if (line.find("MemTotal:") != std::string::npos) {
        std::istringstream ss(line);
        std::string key;
        ss >> key >> total; // Read the value after "MemTotal:"
        break;
      }
    }
    return total; // in kB
  }
  int getFreeMemory() const  {
    std::ifstream mem_info("/proc/meminfo");
    std::string line;
    int free = 0;
    while (std::getline(mem_info, line)) {
      if (line.find("MemFree:") != std::string::npos) {
        std::istringstream ss(line);
        std::string key;
        ss >> key >> free; // Read the value after "MemFree:"
        break;
      }
    }
    return free; // in kB
  }
private:
  int total_memory;
  int free_memory;
  int used_memory;
  int memory_usage;
};