#include "stringUtils.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

std::vector<std::string> splitStringIntoArrayUsingDelim(std::string str, std::string delim){
    //id +++ tag +++ words +++ pri
    //0  +++  1  +++   2   +++  3
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