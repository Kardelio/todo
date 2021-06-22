#include <iostream>
#include <regex>
#include <stdlib.h>
#include "googleDrive.h"

using namespace std;

std::string GoogleDrive::DRIVE_FILE_NAME = ".todo";

std::string GoogleDrive::REFRESH_TOKEN_KEY = "DRIVE_REFRESH_TOKEN";
std::string GoogleDrive::CLIENT_ID_KEY = "DRIVE_CLIENT_ID";
std::string GoogleDrive::CLIENT_SECRET_KEY = "DRIVE_CLIENT_SECRET";


GoogleDrive::GoogleDrive(){
    m_refresh_token = getEnvVariable(GoogleDrive::REFRESH_TOKEN_KEY);
    m_client_id = getEnvVariable(GoogleDrive::CLIENT_ID_KEY);
    m_client_secret = getEnvVariable(GoogleDrive::CLIENT_SECRET_KEY);
    //cout << "---" << m_refresh_token << "---" << endl;
    //cout << "---" << m_client_id << "---" << endl;
    //cout << "---" << m_client_secret << "---" << endl;
    m_access_token = "";
}

std::string GoogleDrive::getEnvVariable(string envvar){
    char * pEnv;
    if (( pEnv = getenv(envvar.c_str())) == NULL )
    {
        cout << "Env variable " << envvar << " does not exist" << endl;
        exit(1);
        return "";
    }
    else
    {
        string strMethod( getenv(envvar.c_str()));
        return strMethod.c_str();
    } 
}

bool GoogleDrive::authenticate(){
    string curlCommand = "curl -X POST -s --data \
                          'client_id="+m_client_id+"&client_secret="+m_client_secret+"&refresh_token="+m_refresh_token+"&grant_type=refresh_token' \
                          https://accounts.google.com/o/oauth2/token";

    string response = CommandRunner::exec(curlCommand);

    regex rgx("access_token\": \"(.*)\"");
    smatch match;
    if (regex_search(response, match, rgx)){
        //whole match
        //std::cout << "match: " << match[0] << '\n';
        //just the group i wnat
        string matched = match[1];
        if(!matched.empty()){
            m_access_token = matched;
            return true;
        } else {
            cout << "Match is empyt" << endl;
            return false;
        }
    } else {
        cout << response << endl;
        return false;
    }
}

string GoogleDrive::doesFileExistAlready(){
    string curlCommand = "curl -s -H \"Authorization: Bearer "+m_access_token+"\" https://www.googleapis.com/drive/v3/files";
    string response = CommandRunner::exec(curlCommand);
    regex rgx("name\": \""+DRIVE_FILE_NAME+"\"");
    regex idRgx("id\": \"(.*)\"");
    smatch match, idMatch;
    if (regex_search(response, match, rgx)){
        string matched = match[0];
        if(!matched.empty()){
            if (regex_search(response, idMatch, idRgx)){
                return idMatch[1];
            } else {
                return "";
            }
        } else {
            return "";
        }
    } else {
        return "";
    }
}

bool GoogleDrive::updateFileData(string fileId, string data){
    string request = "curl -X PATCH -s ";
    request += "-H \"Authorization: Bearer "+m_access_token+"\" ";
    request += "-H \"Content-Type: multipart/related; boundary=foo_bar_baz\" ";
    request += "-H \"Cache-Control: no-cache\" ";
    request += "-d '--foo_bar_baz\n";
    request += "Content-Type: application/json; charset=UTF-8\n";
    request += "\n";
    request += "{\"name\": \""+DRIVE_FILE_NAME+"\"}\n";
    request += "\n";
    request += "--foo_bar_baz\n";
    request += "Content-Type: text/plain\n";
    request += "\n";
    request += data+"\n";
    request += "--foo_bar_baz--' ";
    request += "https://www.googleapis.com/upload/drive/v3/files/"+fileId+"?uploadType=multipart";

    string out = CommandRunner::exec(request);
    return true;
}

bool GoogleDrive::uploadFileData(string data){
    string request = "curl -X POST -s ";
    request += "-H \"Authorization: Bearer "+m_access_token+"\" ";
    request += "-H \"Content-Type: multipart/related; boundary=foo_bar_baz\" ";
    request += "-H \"Cache-Control: no-cache\" ";
    request += "-d '--foo_bar_baz\n";
    request += "Content-Type: application/json; charset=UTF-8\n";
    request += "\n";
    request += "{\"name\": \""+DRIVE_FILE_NAME+"\"}\n";
    request += "\n";
    request += "--foo_bar_baz\n";
    request += "Content-Type: text/plain\n";
    request += "\n";
    request += data+"\n";
    request += "--foo_bar_baz--' ";
    request += "https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart";

    string out = CommandRunner::exec(request);
    return true;
}

void GoogleDrive::uploadUpdatedTodoFile(string data){
    if(authenticate()){
        string id = doesFileExistAlready();
        if(!id.empty()){
            updateFileData(id, data);
            cout << DRIVE_FILE_NAME << " has been updated in your Google Drive" << endl;
        } else {
            uploadFileData(data);
            cout << DRIVE_FILE_NAME << " has been created in your Google Drive" << endl;
        }
    } else {
        cout << "Auth failed for some reason" << endl;
    }
}

string GoogleDrive::getRemoteVersion(){
    if(authenticate()){
        string id = doesFileExistAlready();
        if(!id.empty()){
            string curlCommand = "curl -s -H \"Authorization: Bearer "+m_access_token+"\" https://www.googleapis.com/drive/v3/files/"+id+"?alt=media";
            string response = CommandRunner::exec(curlCommand);
            return response;
        }
        return "";
    } else {
        cout << "Auth failed for some reason" << endl;
        return "";
    }
}

bool GoogleDrive::areLocalAndRemoteSame(string remoteData, string localData){
    return true;
}
