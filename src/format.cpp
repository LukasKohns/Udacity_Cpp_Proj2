#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ZeroHelper(int t) {
    if (t<10) {
        return "0" + to_string(t);
    }
    else{
        return to_string(t);
    }
}

string Format::ElapsedTime(long seconds) { 
    int tot_mins = seconds / 60;
    int hours = tot_mins / 60;

    int min = tot_mins % 60;
    int sec = seconds % 60;

    string Time = Format::ZeroHelper(hours) + " : " + Format::ZeroHelper(min) + " : " + Format::ZeroHelper(sec) ;


    return Time; 
}