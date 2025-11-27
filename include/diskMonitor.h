#pragma once
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>
#include <sys/statvfs.h>

using json = nlohmann::json;

class DiskMonitor{
public:
  json operator()(){
    json j;
    j["total_disk"] = total_disk;
    j["used_disk"] = used_disk;
    j["free_disk"] = free_disk;
    j["disk_usage"] = disk_usage;
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
    disk_usage = (used_disk * 100) / total_disk;
  }
  int total_disk;
  int used_disk;
  int free_disk;
  int disk_usage;
};