#pragma once 

#include <fstream>
#include <string>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class NetworkMonitor {
public:
  json operator()(){
    json j;
    j["total_bytes_sent"] = total_bytes_sent;
    j["total_bytes_received"] = total_bytes_received;
    j["bytes_sent_per_sec"] = bytes_sent_per_sec;
    j["bytes_received_per_sec"] = bytes_received_per_sec;
    return j;
  }
  NetworkMonitor(){
    getNetworkUsage();
  }
private:
  void getNetworkUsage(){
    std::ifstream file("/proc/net/dev");
    std::string line;
    total_bytes_sent = 0;
    total_bytes_received = 0;
    // Skip the first two lines
    std::getline(file,line);
    std::getline(file,line);
    while(std::getline(file,line)){
      std::istringstream ss (line);
      std::string interface;
      unsigned long bytes_received,bytes_sent;
      ss >> interface;
      ss >> bytes_received;
      for(int i =0;i<7;i++){
        ss >> std::ws; // Skip unused fields
      }
      ss >> bytes_sent;
      total_bytes_received += bytes_received;
      total_bytes_sent += bytes_sent;
    }
    // For simplicity, we set bytes per second to total bytes for now
    bytes_sent_per_sec = total_bytes_sent;
    bytes_received_per_sec = total_bytes_received;
  }
  unsigned long total_bytes_sent;
  unsigned long total_bytes_received;
  unsigned long bytes_sent_per_sec;
  unsigned long bytes_received_per_sec;
};