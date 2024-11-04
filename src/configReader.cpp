#include <algorithm>
#include "configReader.h"
#include "identifier.h"
#include "stringUtils.h"
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

std::string ConfigReader::configFileName = ".todocfg";

ConfigReader::ConfigReader(){
}

std::string ConfigReader::getConfigFullFileLocation(){
    const char* homeDir = getenv("HOME");
    std::string s(homeDir);
    s.append("/"+configFileName);
    return s;
}

void ConfigReader::installTodo(){
    //cout << "Checking todo is installed properly..." << endl;
    /*
     * Make sure folde ris there
     */
}

void ConfigReader::checkForExistingConfig(){
  std::ifstream ifile(getConfigFullFileLocation().c_str());
  if(!(bool)ifile){
      cout << "Config file doesn't exist yet, copying it to HOME dir now" << endl;
      system(("cp "+configFileName+" $HOME").c_str());
  }
}

void ConfigReader::readConfigFileIn(){
    std::ifstream cFile (getConfigFullFileLocation().c_str());
    if (cFile.is_open())
    {
        std::string line;
        while(getline(cFile, line)){
            line.erase(remove_if(line.begin(), line.end(), ::isspace),
                                 line.end());
            if(line[0] == '#' || line.empty())
                continue;
            auto delimiterPos = line.find("=");
            auto name = line.substr(0, delimiterPos);
            auto value = line.substr(delimiterPos + 1);
            m_configMap.insert(std::make_pair(name,value));
        }
    }
    else {
        std::cerr << "Couldn't open config file for reading.\n";
    }
}

std::string ConfigReader::getConfigValueForKey(std::string key){
	map<std::string,std::string>::iterator it = m_configMap.find(key);
	if(it != m_configMap.end()){
		return it->second;
	}
	return "";
}   

std::vector<Identifier> ConfigReader::getLists(){
    vector<Identifier> listOfLists;
    string val = getConfigValueForKey("lists");
    vector<string> allLists = splitStringIntoArrayUsingDelim(val,"-");
    for(size_t i = 0; i < allLists.size(); i++)
    {
        vector<string> eachElm = splitStringIntoArrayUsingDelim(allLists.at(i),",");
        int id = stoi(eachElm.at(0));
        string label = eachElm.at(1);
        string col = eachElm.at(2);
        listOfLists.push_back(Identifier(id,label,col));
    }
    return listOfLists;
}

std::vector<Identifier> ConfigReader::getPriorities(){
    vector<Identifier> listOfPri;
    string val = getConfigValueForKey("priorities");
    vector<string> allLists = splitStringIntoArrayUsingDelim(val,"-");
    for(size_t i = 0; i < allLists.size(); i++)
    {
        vector<string> eachElm = splitStringIntoArrayUsingDelim(allLists.at(i),",");
        int id = stoi(eachElm.at(0));
        string label = eachElm.at(1);
        string col = eachElm.at(2);
        listOfPri.push_back(Identifier(id,label,col));
    }
    return listOfPri;
}
