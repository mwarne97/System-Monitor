#include "processor.h"

//Added In
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  long jiffies_active = LinuxParser::ActiveJiffies();
  long jiffies_total = LinuxParser::Jiffies();
  util_ = static_cast<float>(jiffies_active) / static_cast<float>(jiffies_total);
  return util_; 
}
