#include "processor.h"
#include <string>
#include <sstream>
#include "linux_parser.h"
using std::string;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  string trash, user_str, nice_str, system_str, idle_str, iowait_str, irq_str, softirq_str, steal_str, guest_str, guestnice_str;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> trash >> user_str >> nice_str >> system_str >> idle_str >> iowait_str >> irq_str >> softirq_str >> steal_str >> guest_str >> guestnice_str;
  }
  float work = std::stof(user_str) + std::stof(nice_str) + std::stof(system_str) + std::stof(irq_str) + std::stof(softirq_str) + std::stof(steal_str);
  float idle = std::stof(idle_str) + std::stof(iowait_str);
  float total = work + idle;
  
  return work / total; 
}