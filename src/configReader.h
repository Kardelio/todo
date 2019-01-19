#pragma once
#include <map>
#include <string>
#include <vector>
#include "identifier.h"

class ConfigReader{
    private:
        static std::string configFileName;
        std::map<std::string,std::string> m_configMap;
        std::string getConfigFullFileLocation();
    public:
        ConfigReader();
        void checkForExistingConfig();
        void readConfigFileIn();
        std::string getConfigValueForKey(std::string key);
        std::vector<Identifier> getLists();     
        std::vector<Identifier> getPriorities();
};
