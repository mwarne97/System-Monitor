#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


Processor& System::Cpu() { 
  cpu_ = Processor();
  return cpu_; 
}

vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  for(int pid : pids){
    Process process(pid);
    processes_.emplace_back(process);
  }//End For
  return processes_; 
}

std::string System::Kernel() { 
  kernel_ = LinuxParser::Kernel();
  return kernel_; 
}

float System::MemoryUtilization() { 
  mem_util_ = LinuxParser::MemoryUtilization();
  return mem_util_; 
}

std::string System::OperatingSystem() { 
  osystem_ = LinuxParser::OperatingSystem();
  return osystem_; 
}

int System::RunningProcesses() { 
  running_processes_ = LinuxParser::RunningProcesses();
  return running_processes_; 
}

int System::TotalProcesses() { 
  total_processes_ = LinuxParser::TotalProcesses();
  return total_processes_; 
}

long int System::UpTime() { 
  uptime_ = LinuxParser::UpTime();
  return uptime_; 
}
