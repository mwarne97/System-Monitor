#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      

 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string kernel_{""};
  std::string osystem_{""};
  long int uptime_{0};
  int total_processes_{0};
  int running_processes_{0};
  float mem_util_{0.0};
};

#endif
