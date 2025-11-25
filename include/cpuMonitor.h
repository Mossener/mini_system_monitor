#include <iostream>
#include <fmt/format.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class cpuMonitor{
public:
  operator json()const{
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
};
