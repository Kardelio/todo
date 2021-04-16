#pragma once
#include <iostream>
#include "commandRunner.h"

class GoogleDrive{
    private:
        //from env to start with?
        std::string m_refresh_token;
        std::string m_client_id;
        std::string m_client_secret;
        static std::string DRIVE_FILE_NAME;
        static std::string REFRESH_TOKEN_KEY;
        static std::string CLIENT_ID_KEY;
        static std::string CLIENT_SECRET_KEY;
        std::string getEnvVariable(std::string envvar);
        bool authenticate();
        std::string doesFileExistAlready();
        bool uploadFileData(std::string data);
        bool updateFileData(std::string fileId, std::string data);
        std::string m_access_token;
    public: GoogleDrive();
        void uploadUpdatedTodoFile(std::string data);
        std::string getRemoteVersion();
        bool areLocalAndRemoteSame(std::string remoteData, std::string localData);
};
