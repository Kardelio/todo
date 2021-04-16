#include "commandRunner.h"
#include <fstream>
#include <iostream>
#include <unistd.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;
    
void CommandRunner::runCommand(std::string command){
    int status = system(command.c_str());
    if(status != 0){
        cout << "Command Failed" << endl;
    }
} 

void CommandRunner::clearScreen(){
    system("clear");
}

string CommandRunner::exec(string command) {
   char buffer[128];
   string result = "";

   // Open pipe to file
   FILE* pipe = popen(command.c_str(), "r");
   if (!pipe) {
      return "popen failed!";
   }

   // read till end of process:
   while (!feof(pipe)) {

      // use buffer to read and add to result
      if (fgets(buffer, 128, pipe) != NULL)
         result += buffer;
   }

   pclose(pipe);
   return result;
}
