#include <string>
#include <sstream>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  int h, m, s;
  
  std::stringstream stream;
  
  h = (seconds / 3600);
  
  seconds = seconds % 3600;
  m = (seconds / 60);
  
  s = seconds % 60;
  
  if(h < 10){
    stream << "0";
  }//End If
  
  stream << std::to_string(h) << ":";
  
  if(m < 10){
    stream << "0";
  }//End If
    
  stream << std::to_string(m) << ":";
    
  if(s < 10){
    stream << "0";
  }//End If
  
  stream << std::to_string(s);
  
  string long_time = stream.str();
  
  return long_time;
}
