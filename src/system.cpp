#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"

//Added In
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { 
  cpu_ = Processor();
  return cpu_; 
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> pids = LinuxParser::Pids();
  for(int pid : pids){
    Process process(pid);
    processes_.push_back(process);
  }//Emd For
  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { 
  kernel_ = LinuxParser::Kernel();
  return kernel_; 
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { 
  mem_util_ = LinuxParser::MemoryUtilization();
  return mem_util_; 
}

// TODO: Return the operating system name
std::string System::OperatingSystem() { 
  osystem_ = LinuxParser::OperatingSystem();
  return osystem_; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { 
  running_processes_ = LinuxParser::RunningProcesses();
  return running_processes_; 
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { 
  total_processes_ = LinuxParser::TotalProcesses();
  return total_processes_; 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { 
  uptime_ = LinuxParser::UpTime();
  return uptime_; 
}
