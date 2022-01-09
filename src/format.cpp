#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int tot_mins = seconds / 60;
    int hours = tot_mins / 60;

    int min = tot_mins % 60;
    int sec = seconds % 60;

    string Time = to_string(hours) + " : " + to_string(min) + " : " + to_string(sec) ;


    return Time; 
}