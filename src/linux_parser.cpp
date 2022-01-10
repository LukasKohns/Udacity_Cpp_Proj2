#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == kPreNamKey) {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os;
  string kernel;
  string version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  stream.close();
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key;
  string value;
  string trash;
  float memtotal;
  float memfree;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value >> trash){
        if (key == kMemTotKey){
          memtotal = std::stof(value);
        }
        if (key == kMemFreeKey){
          memfree = std::stof(value);
          break;
        }
      }

    }
  }
  filestream.close();
  return (memtotal - memfree)/memtotal; 
}

// TODO: Read and return the system uptime
long int LinuxParser::UpTime() { 
  string uptime;
  long int uptimel;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    uptimel = std::stol(uptime);
  }
  stream.close();
  return uptimel;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

//Added manually: CPU utilization of process ID
float LinuxParser::CpuUtilization(int pid) {        
  string value; 
  string line;
  long int utime;
  long int stime;
  long int cutime;
  long int cstime;
  long int uptime; 
  int pos = 0;
  int utime_pos = 13; 
  int stime_pos = 14; 
  int cutime_pos =15;
  int cstime_pos = 16;
  
  float cpu_util = 0;
  
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> value && pos < 22) {
        if (pos == utime_pos) {utime = (std::stol(value));}
        if (pos == stime_pos) {stime = (std::stol(value));}
        if (pos == cutime_pos) {cutime = (std::stol(value));}
        if (pos == cstime_pos) {cstime = (std::stol(value));}
        pos++;
      }
      break;      
    }
  //Problem is that sys uptime is in secs
  uptime = LinuxParser::UpTime(pid);
  // I used the given Stackoverflow-Link for this calculation
  // total = utime + stime
  long total_time = utime + stime;
  //total with wait = total + cu +cs
  long total_time_with_wait = total_time + cutime + cstime; 
  long total_tww_secs = total_time_with_wait / sysconf(_SC_CLK_TCK);
  //Utilization = total with wait / uptime
  cpu_util =  static_cast<float>(total_tww_secs) / static_cast<float>(uptime);
  }
  filestream.close();  
  return cpu_util;
}      
   
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  string value;
  int tot_process;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == kProcKey){
          tot_process = std::stoi(value);
          break;
        }
      }
    }
  }
  filestream.close();
  return tot_process; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == kProRunKey){
          return std::stoi(value);
        }
      }
    }
  } 
  filestream.close();
  return std::stoi(value);
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string pidpath = kProcDirectory + to_string(pid) + kCmdlineFilename;

  string line;
  std::ifstream stream(pidpath);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
    
  }
  stream.close();
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
  string pidpath = kProcDirectory + to_string(pid) + kStatusFilename;

  string line;
  string key;
  string value; 
  string value_MB_str;
  int value_kB;
  std::ifstream filestream(pidpath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == kVmKey){
          value_kB = std::stoi(value);
          value_MB_str = to_string(value_kB / 1000);
          return value_MB_str;
        }
      }
    }
  }
  filestream.close();
  return value_MB_str; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string pidpath = kProcDirectory + to_string(pid) + kStatusFilename;

  string line;
  string key;
  string value;
  std::ifstream filestream(pidpath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == kUidKey){
          return value;
        }
      }
    }
  }
  filestream.close();
  return value; 
  
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line; 
  string key;
  string value;
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> value >> key) {
        if (key == uid ) {
          return value;
        }
      }
    }
  }
  filestream.close();
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string pidpath = kProcDirectory + to_string(pid) + kStatFilename;
  
  string line, value;
  vector<string> values;

  std::ifstream stream(pidpath);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  stream.close();
  return UpTime() - stol(values[21])/sysconf(_SC_CLK_TCK);
}
