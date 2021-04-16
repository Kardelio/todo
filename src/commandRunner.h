#pragma once
#include <string>

class CommandRunner{
    public:
        static void runCommand(std::string command); 
        static void clearScreen(); 
        static std::string exec(std::string command);
};
