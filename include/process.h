#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               
  std::string User();                      
  std::string Command();                   
  float CpuUtilization();                  
  std::string Ram();                       
  long int UpTime();                       
  bool operator<(Process const& a) const;  

  Process(int);
  
 private:
  int pid_{0};
  std::string user_{""};
  std::string command_{""};
  float cpu_utilization_{0.0};
  std::string ram_{""};
  long int uptime_{0};
};

#endif
