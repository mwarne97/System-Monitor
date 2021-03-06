#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;


string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  float mem_total{0.0}, mem_free{0.0};
  float mem_util{0.0};
  
  string key{""}, value{""};
  string line{""};
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream line_stream(line);
      line_stream >> key >> value;
      if(key == "MemTotal:"){
        mem_total = stof(value);
      } else if (key == "MemFree:"){
        mem_free = stof(value);
      }//End If Else
    }//End While
    mem_util = (mem_total - mem_free) / mem_total;
  }//End If
  return mem_util;
}

long LinuxParser::UpTime() { 
  string line{""};
  string uptime{""};
  
  long uptime_long;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream uptime_stream(line);
    uptime_stream >> uptime;
    uptime_long = std::stol(uptime);
  }//End If
  
  return uptime_long;
}

long LinuxParser::Jiffies() { 
  vector<string> cpu_times_strings = CpuUtilization();
  vector<long> cpu_times_longs;
  long jiffies_total = 0;
  
  for(string time_str : cpu_times_strings){
    long time_long = std::stol(time_str);
    cpu_times_longs.emplace_back(time_long);
    jiffies_total += time_long;
  }//End For
  
  return jiffies_total; 
}

long LinuxParser::ActiveJiffies(int pid) { 
  string line{""};
  
  string utime{""}, stime{""};
  string cutime{""}, cstime{""};
  
  long total_time{0};
  
  int counter{0};
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream istream(line);
    while(counter < 14){
      istream >> utime;
      counter++;
    }//End While
    istream >> stime >> cutime >> cstime;
    total_time = std::stol(utime) + std::stol(stime) + std::stol(cutime) + std::stol(cstime);
  }//End If
  
  return total_time;
}

long LinuxParser::ActiveJiffies() { 
  vector<string> cpu_times_strings = CpuUtilization();
  vector<long> cpu_times_longs;
  
  for(string time_str : cpu_times_strings){
    long time_long = std::stol(time_str);
    cpu_times_longs.emplace_back(time_long);
  }//End For
  
  long jiffies_active = cpu_times_longs[CPUStates::kUser_] + cpu_times_longs[CPUStates::kNice_] + cpu_times_longs[CPUStates::kSystem_] + cpu_times_longs[CPUStates::kIRQ_] + cpu_times_longs[CPUStates::kSoftIRQ_] + cpu_times_longs[CPUStates::kSteal_];
  
  return jiffies_active; 
}

long LinuxParser::IdleJiffies() { 
  vector<string> cpu_times_strings = CpuUtilization();
  vector<long> cpu_times_longs;
  
  for(string time_str : cpu_times_strings){
    long time_long = std::stol(time_str);
    cpu_times_longs.emplace_back(time_long);
  }//End For
  
  long jiffies_idle = cpu_times_longs[CPUStates::kIdle_] + cpu_times_longs[CPUStates::kIOwait_];
  
  return jiffies_idle;
}

vector<string> LinuxParser::CpuUtilization() { 
  string key{""}, value{""};
  string line{""};
  
  vector<string> cpu_times{};
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream istring_stream(line);
    istring_stream >> key;
    while(istring_stream >> value){
      cpu_times.emplace_back(value);
    }//End While
  }//End If
  return cpu_times;
}

int LinuxParser::TotalProcesses() { 
  string line{""};
  
  string key{""};
  int value{0};
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream istream(line);
      while(istream >> key >> value){
        if(key == "processes"){
          return value;
        }//End If
      }//End While
    }//End While
  }//End If
  return value; 
}

int LinuxParser::RunningProcesses() { 
  string line{""};
  
  string key{""};
  int value{0};
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream istream(line);
      while(istream >> key >> value){
        if(key == "procs_running"){
          return value;
        }//End If
      }//End While
    }//End While
  }//End If
  return value; 
}

string LinuxParser::Command(int pid) { 
  string line{""};
  
  string command{""};
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream istream(line);
    istream >> command;
  }//End If
  return command;
}

string LinuxParser::Ram(int pid) { 
  string line{""};
  
  string key{""};
  long value{0};
  string value_str{""};
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream istream(line);
      istream >> key;
      if(key == "VmRSS:"){
        // "VmRSS" was used to give the exact physical memory being used 
        // compared to "VMSize" which gives the sum of all virtual memory 
        // My first reviewer suggested that I make the change
        istream >> value;
        break;
      }//End If
    }//End While
    value_str = to_string(value / 1024);
  }//End If
  return value_str;
}

string LinuxParser::Uid(int pid) { 
  string line{""};
  
  string key{""}, uid{""};
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      stream >> key;
      if(key == "Uid:"){
        stream >> uid;
      }//End If
    }//End While
  }//End If
  return uid; 
}

string LinuxParser::User(int pid) { 
  string line{""};
  string uid = Uid(pid);
  
  string name{""};
  char x;
  string value{""};
    
  std::ifstream stream(kPasswordPath);
  while(std::getline(stream, line)){
    for(int counter = 0; counter < line.length(); counter++){
      if(line[counter] == ':'){
        line[counter] = ' ';
      }//End If
    }//End For
    std::istringstream istream(line);
    istream >> name >> x >> value;
    if(value == uid){
      return name;
    }//End If
  }//End While
  return name; 
}

long LinuxParser::UpTime(int pid) { 
  string line{""};
  
  string value{""};
  long value_long{0};
  
  int counter = 0;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  
  if(stream.is_open()){
    std::getline(stream, line);
    std::istringstream istream(line);
    while(counter < 22){
      istream >> value;
      counter++;
    }//End While
    value_long = std::stol(value);
  }//End If
  return value_long; 
}
