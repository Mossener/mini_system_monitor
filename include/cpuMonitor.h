#include <iostream>
#include <fstream>
#include <string>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class cpuMonitor {
public:
  cpuMonitor() {
    // 在构造函数中获取 CPU 信息
    cpu_count = getCputCount();
    cpu_freq = getCpuFreq();
    cpu_usage = getCpuUsage();
  }

  json operator()() const {
    json j;
    json j_data;
    j["type"] = "cpu";
    j_data["count"] = cpu_count;
    j_data["freq"] = cpu_freq;
    j_data["usage"] = cpu_usage;
    j["data"] = j_data;
    return j;
  }

private:
  int cpu_count;
  int cpu_freq;
  int cpu_usage;

private: 
  int getCputCount() const {
    std::ifstream cpu_info("/proc/cpuinfo");
    std::string line;
    int count = 0;
    while (std::getline(cpu_info, line)) {
      if (line.find("processor") != std::string::npos) {
        count++;
      }
    }
    return count;
  }

  int getCpuFreq() const  {
    std::ifstream cpu_info("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    int freq = 0;
    cpu_info >> freq;
    return freq / 1000; // Convert to MHz
  }

  int getCpuUsage() const {
    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);
    std::istringstream ss(line);
    std::string cpu;
    int user;
    int nice;
    int system;
    int idle;
    ss >> cpu >> user >> nice >> system >> idle;
    int total = user + nice + system + idle;
    int usage = (total - idle) * 100 / total;
    return usage;
  }
};

