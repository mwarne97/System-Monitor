#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

int Process::Pid() {
  return pid_; 
}

float Process::CpuUtilization() { 
  long hertz = sysconf(_SC_CLK_TCK);
  
  long total_time_seconds = LinuxParser::ActiveJiffies(Pid()) / hertz;
  
  long seconds = Process::UpTime();
  
  cpu_utilization_ = (static_cast<float>(total_time_seconds) / static_cast<float>(seconds));
  
  return cpu_utilization_;
}

string Process::Command() { 
  command_ = LinuxParser::Command(Pid());
  return command_; 
}

string Process::Ram() { 
  ram_ = LinuxParser::Ram(Pid());
  return ram_; 
}

string Process::User() { 
  user_ = LinuxParser::User(Pid());
  return user_;
}

long int Process::UpTime() { 
  long uptime_system_seconds = LinuxParser::UpTime();
  long uptime_process_ticks = LinuxParser::UpTime(Pid());
  
  long uptime_process_seconds = uptime_process_ticks / sysconf(_SC_CLK_TCK);
  
  uptime_ = uptime_system_seconds - uptime_process_seconds;
  return uptime_; 
}

bool Process::operator<(Process const& a) const { 
  return std::stol(ram_) < std::stol(a.ram_);
}
