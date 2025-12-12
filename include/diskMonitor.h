#pragma once
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <sys/statvfs.h>

using json = nlohmann::json;

class DiskMonitor{
public:
  json operator()(){
    json j,j_data;
    j["type"] = "disk";
    j_data["total"] = total_disk;
    j_data["used"] = used_disk;
    j_data["free"] = free_disk;
    j_data["usage"] = 0;
    j["data"] = j_data;

    return j;
  }
  DiskMonitor(){
    getDiskUsage();
  }
private:
  void getDiskUsage(){
    std::ifstream file("/proc/mounts");
    std::string line;
    total_disk = 0;
    used_disk = 0;
    free_disk = 0;
    struct statvfs stat;
    while(std::getline(file,line)){
      std::istringstream ss (line);
      std::string device,mountpoint,filesystem;
      ss >> device >> mountpoint >> filesystem;
      if(statvfs(mountpoint.c_str(),&stat) != 0){
        continue;
      }
    }
    total_disk = (stat.f_blocks * stat.f_frsize) / 1024;
    free_disk = (stat.f_bfree * stat.f_frsize) / 1024;
    used_disk = total_disk - free_disk;
  }
  int total_disk;
  int used_disk;
  int free_disk;
  int disk_usage;
};