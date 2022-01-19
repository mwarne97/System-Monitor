#include "processor.h"

#include "linux_parser.h"


float Processor::Utilization() { 
  long jiffies_active = LinuxParser::ActiveJiffies();
  long jiffies_total = LinuxParser::Jiffies();
  util_ = static_cast<float>(jiffies_active) / static_cast<float>(jiffies_total);
  return util_; 
}
