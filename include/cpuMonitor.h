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

  operator json() const {
    json j;
    j["cpu_count"] = cpu_count;
    j["cpu_freq"] = cpu_freq;
    j["cpu_usage"] = cpu_usage;
    return j;
  }

private:
  int cpu_count;
  int cpu_freq;
  int cpu_usage;

private: 
  int getCputCount() {
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

  int getCpuFreq() {
    std::ifstream cpu_info("/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq");
    int freq = 0;
    cpu_info >> freq;
    return freq / 1000; // Convert to MHz
  }

  int getCpuUsage() {
    std::ifstream stat_file("/proc/stat");
    std::string line;
    std::getline(stat_file, line);
    std::istringstream ss(line);
    std::string cpu;
    int user, nice, system, idle;
    ss >> cpu >> user >> nice >> system >> idle;
    int total = user + nice + system + idle;
    int usage = (total - idle) * 100 / total;
    return usage;
  }
};

