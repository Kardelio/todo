#include "stringUtils.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>

std::vector<std::string> splitStringIntoArrayUsingDelim(std::string str, std::string delim){
    std::string s = str;
    std::string delimiter = delim;

    size_t pos = 0;

    std::string token;
    std::vector<std::string> results;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        results.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    results.push_back(s);

    return results;
}


std::string getCurrentTimeStr(){
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    //http://www.cplusplus.com/reference/ctime/tm/
    return ""+std::to_string(now->tm_hour)+":"
        +std::to_string(now->tm_min)+":"
        +std::to_string(now->tm_sec)+"_"
        +std::to_string(now->tm_mday)+"-"
        +std::to_string(now->tm_mon + 1)+"-"
        +std::to_string((now->tm_year + 1900))+"";
}
