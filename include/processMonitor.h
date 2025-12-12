#pragma once 

#include <string>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <vector>
#include <chrono>
#include <ctime>
#include <iostream>
#include <algorithm>
#include <numeric>
using json = nlohmann::json;
namespace filesystem = std::filesystem;
//检测所有进程的运行状态

struct Stat
{
  int pid;
  std::string name;
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  int flags;
  unsigned long minflt;
  unsigned long cminflt;
  unsigned long majflt;
  unsigned long cmajflt;
  unsigned long utime;
  unsigned long stime;
  long cutime;
  long cstime;
  long priority;
  long nice;
  long num_threads;
  long itrealvalue;
  unsigned long starttime;
  unsigned long vsize;
  long rss;
  unsigned long rsslim;
  unsigned long startcode;
  unsigned long endcode;
  unsigned long startstack;
  unsigned long kstkesp;
  unsigned long signal;
  unsigned long blocked;
  unsigned long sigignore;
  unsigned long sigcatch;
  unsigned long wchan;
  unsigned long nswap;
  unsigned long cnswap;
  int exit_signal;
  int processor;
  unsigned long rt_priority;
  unsigned long policy;
  unsigned long delayacct_blkio_ticks;
  unsigned long guest_time;
  long cguest_time;
};

struct Statm{
  unsigned long size;
  unsigned long resident;
  unsigned long share;
  unsigned long text;
  unsigned long lib;
  unsigned long data;
  unsigned long dt;
};

class ProcessMonitor {
public:
  ProcessMonitor() = default;
  ~ProcessMonitor() = default;
  json operator()(){
    json j = json::object();
    std::vector<int>pid_array;
    filesystem::path dir = "/proc";

    for(auto & file_defination : filesystem::directory_iterator(dir)){
      if(file_defination.is_directory()){
        std::string dir_name = file_defination.path().filename().string();

        if(std::all_of(dir_name.begin(), dir_name.end(),::isdigit)){
          pid_array.push_back(std::stoi(dir_name));
        }
      }
    }
    for(auto & pid : pid_array){
      std::string status_file_path = "/proc/" + std::to_string(pid);
      std::string cpu_usage_file_path = status_file_path + "/stat";
      std::string memory_usage_file_path = status_file_path + "/status";
      std::string io_usage_file_path = status_file_path + "/net/dev";
      Stat stat = getCpuTime(cpu_usage_file_path);
      Statm statm = getMemoryUsage(memory_usage_file_path);
      std::string pid_key = std::to_string(pid);
      j[pid_key]["name"] = stat.name;
      j[pid_key]["state"] = stat.state;
      j[pid_key]["ppid_key"] = stat.ppid;
      j[pid_key]["utime"] = stat.utime;
      j[pid_key]["stime"] = stat.stime;
      j[pid_key]["cutime"] = stat.cutime;
      j[pid_key]["cstime"] = stat.cstime;
      j[pid_key]["priority"] = stat.priority;
      j[pid_key]["nice"] = stat.nice;
      j[pid_key]["num_threads"] = stat.num_threads;
      j[pid_key]["starttime"] = stat.starttime;
      j[pid_key]["rss"] = stat.rss;
      j[pid_key]["policy"] = stat.policy;
      j[pid_key]["size"] = statm.size;
    }
    return j;
  }
private:
  Stat getCpuTime(std::string file_path){
    Stat stat;
    std::ifstream ifs(file_path);
    if(ifs.is_open()){
      ifs >> stat.pid >> stat.name >> stat.state >> stat.ppid >> stat.pgrp >> stat.session >> stat.tty_nr >> stat.tpgid >> stat.flags >> stat.minflt >> stat.cminflt >> stat.majflt >> stat.cmajflt >> stat.utime >> stat.stime >> stat.cutime >> stat.cstime >> stat.priority >> stat.nice >> stat.num_threads >> stat.itrealvalue >> stat.starttime >> stat.vsize >> stat.rss >> stat.rsslim >> stat.startcode >> stat.endcode >> stat.startstack >> stat.kstkesp >> stat.signal >> stat.blocked >> stat.sigignore >> stat.sigcatch >> stat.wchan >> stat.nswap >> stat.cnswap >> stat.exit_signal >> stat.processor >> stat.rt_priority >> stat.policy >> stat.delayacct_blkio_ticks >> stat.guest_time >> stat.cguest_time;
    }
    return stat;
  }
  Statm getMemoryUsage(std::string file_path){
    Statm statm;
    std::ifstream ifs(file_path);
    if(ifs.is_open()){
      ifs >> statm.size >> statm.resident >> statm.share >> statm.text >> statm.lib >> statm.data >> statm.dt;
    }
    return statm;
  }
};