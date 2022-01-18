#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

//Added
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

// TODO: Return this process's ID
int Process::Pid() {
  return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  long hertz = sysconf(_SC_CLK_TCK);
  
  long total_time_seconds = LinuxParser::ActiveJiffies(Pid()) / hertz;
  
  long uptime = LinuxParser::UpTime();
  
  long seconds = uptime - Process::UpTime();
  
  cpu_utilization_ = (total_time_seconds / seconds);
  
  return cpu_utilization_; 
}

// TODO: Return the command that generated this process
string Process::Command() { 
  command_ = LinuxParser::Command(Pid());
  return command_; 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  ram_ = LinuxParser::Ram(Pid());
  return ram_; 
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
  user_ = LinuxParser::User(Pid());
  return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  long uptime_system_seconds = LinuxParser::UpTime();
  long uptime_process_ticks = LinuxParser::UpTime(Pid());
  
  long uptime_process_seconds = uptime_process_ticks / sysconf(_SC_CLK_TCK);
  
  uptime_ = uptime_system_seconds - uptime_process_seconds;
  return uptime_; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  return std::stol(ram_) < std::stol(a.ram_);
}
