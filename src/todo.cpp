#include <algorithm>
#include <iostream>
#include "configReader.h"
#include "help.cpp"
#include "stringUtils.h"
#include "todoFileHandler.h"
#include "identifier.h"
#include "todoItem.h"
#include <vector>
#include <list>
#include <fstream>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

// Prototyping functions...
bool isThereText(char *argv[], int argc, string thisOpt);
void readAllTodos();
void readTodosInSingleList(int listNum);
void readTodosInSinglePriority(int priNum);
void addATodo(string thing); void addATDItemToFile(TodoItem item, string file);
void deleteSingleTodo(int id);
TodoItem getSingleTodoItem(int id, string file);
void deleteSingleTodoFromSpecificFile(int id, string file);
void editSingleTodo(int id);
void moveTodo(int id, bool toPrimary);
void clearAllTodos();
void clearScreen();
void drawStartLine();
void printConfigData();
int identifierListContainsId(std::vector<Identifier> vec,int id);
string editInEditor(string title);

// Variable Initialisers...
static string escape = "\u001b";
static string clear = "\u001b[0m";
static string invert = "\u001b[7m";
static string whiteAndBlack = "\u001b[30;47m";
static string whiteText = "\u001b[97m";
int currentPriority = 2;
int currentListToPutIn = 1;
bool displayBacklogToo = false;
string currentTag = "EMPTY";
vector<Identifier> listOfLists;
vector<Identifier> listOfPriorities;
int tagColorFg = 0;
int tagColorBg = 0;
string primaryFile = ".todo";
string secondaryFile = ".todoBacklog";
string logfile = "";
bool shouldLog = false;
ConfigReader *cnfgReader = new ConfigReader();

int main(int argc, char *argv[])
{
    //TODO below is nothing...
    //cnfgReader->installTodo();

    cnfgReader->checkForExistingConfig();
    cnfgReader->readConfigFileIn();
    listOfLists = cnfgReader->getLists();
    listOfPriorities = cnfgReader->getPriorities();
    tagColorFg = stoi(cnfgReader->getConfigValueForKey("tagColorFront"));
    tagColorBg = stoi(cnfgReader->getConfigValueForKey("tagColorBack"));
    primaryFile = cnfgReader->getConfigValueForKey("todoFile");
    secondaryFile = cnfgReader->getConfigValueForKey("todoBackUpFile");

    logfile = cnfgReader->getConfigValueForKey("logFile");
    shouldLog = !logfile.empty();
    TodoFileHandler::setInitialFiles(primaryFile,secondaryFile, logfile);
    //This works nicely
    //TodoFileHandler::writeToLogFile(shouldLog,"This is a test");

    if(argc == 1){
        readAllTodos();
    } else {
        int c;
        while ((c = getopt (argc, argv, "cd:rie:p:l:t:hb:m:sa")) != -1){
            switch (c) {
                case 'c':
                    clearScreen();
                    drawStartLine();
                    readAllTodos();
                    drawStartLine();
                    exit(0);
                    break;
                case 'd':
                    deleteSingleTodo(stoi(optarg));
                    exit(0);
                    break; 
                case 'r':
                    //TODO
                    clearAllTodos();
                    exit(0);
                    break;
                case 'i':
                    printConfigData();
                    exit(0);
                    break;
                case 'e':
                    editSingleTodo(stoi(optarg));
                    exit(0);
                    break;
                case 's':
                    //TODO sync the todo file
                    exit(0);
                    break;
                case 'p':
                    currentPriority = stoi(optarg);
                    if(!isThereText(argv,argc,optarg)){
                        readTodosInSinglePriority(stoi(optarg));
                        exit(0);
                    }
                    break;
                case 'l':
                    currentListToPutIn = stoi(optarg);
                    if(!isThereText(argv,argc,optarg)){
                        readTodosInSingleList(stoi(optarg));
                        exit(0);
                    }
                    break;
                case 't':
                    //currentListToPutIn = stoi(optarg);
                    //if(!isThereText(argv,argc,optarg)){
                    //    readTodosInSingleList(stoi(optarg));
                    //    exit(0);
                    //}
                    // TODO issue
                    currentTag = optarg;
                    break;
                case 'h':
                    Help::printHelp();
                    exit(0);
                    break;
                case 'b':
                    //Move into backlog
                    moveTodo(stoi(optarg),true);
                    exit(0);
                    break;
                case 'm':
                    //Move into Main
                    moveTodo(stoi(optarg),false);
                    exit(0);
                    break;
                case 'a':
                    displayBacklogToo = true;
                    readAllTodos();
                    exit(0);
                    break;
                default:
                    cout << "Invalid Option: " << c << endl;
                    exit(1);
                    break;
            }
        }

        string textToAdd = argv[argc-1];
        textToAdd.erase(std::remove(textToAdd.begin(), textToAdd.end(), '\n'), textToAdd.end());
        if(textToAdd != "" && textToAdd.size() > 0){
            addATodo(textToAdd.c_str());
        }
        readAllTodos();
    }

    return 0;
}

bool isThereText(char *argv[], int argc, string thisOpt){
    string userText = argv[argc-1];
    userText.erase(std::remove(userText.begin(), userText.end(), '\n'), userText.end());
    if(userText != "" && userText.size() > 0 && userText != thisOpt){
        //There is user text
        return true;
    }
    return false;
    //there isnt
}

void addATodo(string thing){
    TodoFileHandler::addTodoItemToFileToSpecificFile(currentListToPutIn,currentTag,thing,currentPriority,TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName));
    TodoFileHandler::writeToLogFile(shouldLog,"ADDED: "+thing);
}

void addATDItemToFile(TodoItem item, string file){
    TodoFileHandler::addactualTDToSpecificFile(item,file);
}

struct less_than_key{
    inline bool operator() (ListItem& struct1, ListItem& struct2){
        return (struct1.getId() < struct2.getId());
    }
};

void readAllTodos(){
    vector<ListItem> list_items = TodoFileHandler::readTodoFileIntoListItems();
    std::sort(list_items.begin(), list_items.end(), less_than_key());
    if(list_items.size() > 0){
        for(size_t i = 0; i < list_items.size(); i++)
        {
            int res = identifierListContainsId(listOfLists,list_items.at(i).getId());
            if(res > -1){
                Identifier item = listOfLists.at(res);
                cout << endl;
                cout << item.getColorWithEscapes() << "    " << item.getId() << ". " << escape << "[1m" << item.getTitle() << "      " << clear << endl;
                
                for(size_t j = 0; j < list_items.at(i).getListOfTodoItems().size(); j++)
                {
                    TodoItem t = list_items.at(i).getListOfTodoItems().at(j);
                    t.printWithFGandBGNew(listOfLists.at(res).getColorWithEscapes(), *cnfgReader,listOfPriorities);
                }
            }
        }
    } else {
        cout << "Empty" << endl;
    }

    cout << endl;

    if(displayBacklogToo){
        cout << escape << whiteAndBlack << "--- BACKLOG ---" << clear << endl;
        vector<TodoItem> backlog_items = TodoFileHandler::readBackLogFileIntoListItems();
        if(backlog_items.size() > 0){
            for(size_t i = 0; i < backlog_items.size(); i++)
            {
                backlog_items.at(i).backlogPrint();
            }
        } else {
            cout << "Empty" << endl;
        }
        cout << endl;
    }
}

void readTodosInSinglePriority(int priNum){
    vector<ListItem> list_items = TodoFileHandler::readTodoFileIntoListItemsWithSinglePriority(priNum);
    if(list_items.size() > 0){
        for(size_t i = 0; i < list_items.size(); i++)
        {
            int res = identifierListContainsId(listOfLists,list_items.at(i).getId());
            if(res > -1){
                Identifier item = listOfLists.at(res);
                cout << endl;
                cout << item.getColorWithEscapes() << "    " << item.getId() << ". " << escape << "[1m" << item.getTitle() << "      " << clear << endl;
                
                for(size_t j = 0; j < list_items.at(i).getListOfTodoItems().size(); j++)
                {
                    TodoItem t = list_items.at(i).getListOfTodoItems().at(j);
                    t.printWithFGandBGNew(listOfLists.at(res).getColorWithEscapes(), *cnfgReader,listOfPriorities);
                }
            }
        }
    } else {
        cout << "Empty" << endl;
    }
    cout << endl;
}

void readTodosInSingleList(int listNum){
    vector<ListItem> list_items = TodoFileHandler::readTodoFileIntoListItemsWithSingleList(listNum);
    if(list_items.size() > 0){
        for(size_t i = 0; i < list_items.size(); i++)
        {
            int res = identifierListContainsId(listOfLists,list_items.at(i).getId());
            if(res > -1){
                Identifier item = listOfLists.at(res);
                cout << endl;
                cout << item.getColorWithEscapes() << "    " << item.getId() << ". " << escape << "[1m" << item.getTitle() << "      " << clear << endl;
                
                for(size_t j = 0; j < list_items.at(i).getListOfTodoItems().size(); j++)
                {
                    TodoItem t = list_items.at(i).getListOfTodoItems().at(j);
                    t.printWithFGandBGNew(listOfLists.at(res).getColorWithEscapes(), *cnfgReader,listOfPriorities);
                }
            }
        }
    } else {
        cout << "Empty" << endl;
    }
    cout << endl;
}

void moveTodo(int id, bool toPrimary){
    string locationTo = ""; 
    string locationFrom = ""; 
    if(toPrimary){
        locationTo = TodoFileHandler::getSpecificConfig(TodoFileHandler::secondaryTodoFileName);
        locationFrom = TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName);
    }else {
        locationTo = TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName);
        locationFrom = TodoFileHandler::getSpecificConfig(TodoFileHandler::secondaryTodoFileName);
    }
    cout << "Moving todo: " << id << " to: " << locationTo << endl;
    TodoItem ite = getSingleTodoItem(id,locationFrom);
    deleteSingleTodoFromSpecificFile(id,locationFrom);
    addATDItemToFile(ite,locationTo);
    readAllTodos();
}

void clearAllTodos(){
    cout << "Are you sure you wish to CLEAR ALL the Todos? ... (y/n)(yes/no)" << endl;
    string answer;
    cin >> answer;
    if(answer == "y" || answer == "Y" || answer == "yes" || answer == "YES"){
        TodoFileHandler::writeFullListToSpecificFile({},TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName));
        readAllTodos();
    } else {
        cout << "Operation Aborted" << endl;
    }
}

void editSingleTodo(int id){
    printConfigData();

    list<TodoItem> list_items = TodoFileHandler::readTodoFileAndGetList(TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName));
    int indxFind = TodoFileHandler::containsTodoItemWithIdInList(list_items,id);
    std::list<TodoItem>::iterator itera = list_items.begin();
    std::advance(itera,indxFind);
    cout << "Currently: " << endl;
    itera->standardPrint();

    cout << escape << "[37;44m" << "Edit the NEW Title (Leave blank to keep it as it is...)" << clear << endl;
    string newTitle = editInEditor(itera->getTitle());
    cout << "-- EDITED IN EDITOR -- " << endl;
    if(newTitle.empty()){
        newTitle = itera->getTitle();
    }
    cout << escape << "[37;44m" << "Edit the NEW Tag (Leave blank to keep it as it is...)" << clear << endl;
    string newTag;
    getline(cin,newTag);
    if(newTag.empty()){
        newTag = itera->getTag();
    }
    cout << escape << "[37;44m" << "Edit the NEW Priority (Leave blank to keep it as it is...)" << clear << endl;
    string newPriority;
    getline(cin,newPriority);
    if(newPriority.empty()){
        newPriority = to_string(itera->getPriority());
    }
    cout << escape << "[37;44m" << "Edit the NEW List (Leave blank to keep it as it is...)" << clear << endl;
    string newList;
    getline(cin,newList);
    if(newList.empty()){
        newList = to_string(itera->getListId());
    }

    itera->setTitle(newTitle);
    itera->setTag(newTag);
    itera->setPriority(stoi(newPriority));
    itera->setListId(stoi(newList));

    TodoFileHandler::writeFullListToSpecificFile(list_items,TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName));
    readAllTodos();
}

void deleteSingleTodo(int id){
    list<TodoItem> list_items = TodoFileHandler::readTodoFileAndGetList(TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName));
    int indxDel = TodoFileHandler::containsTodoItemWithIdInList(list_items,id);
    std::list<TodoItem>::iterator itera = list_items.begin();
    std::advance(itera,indxDel);
    cout << "Deleting : " << itera->getId() << ": "<< itera->getName() << endl;
    TodoFileHandler::writeToLogFile(shouldLog,"DELETED: "+itera->getName());
    list_items.erase(itera);

    std::list<TodoItem>::iterator iter;
    for (iter = list_items.begin(); iter != list_items.end(); ++iter){
        int posNow = std::distance(list_items.begin(), iter);
        if(iter->getId() != posNow){
            iter->setId(posNow);
        }
    }

    TodoFileHandler::writeFullListToSpecificFile(list_items, TodoFileHandler::getSpecificConfig(TodoFileHandler::todoFileName));
    //TODO put deleted ones into own list and write that to the backup
    //just append it
    readAllTodos();
}

TodoItem getSingleTodoItem(int id, string file){
    list<TodoItem> list_items = TodoFileHandler::readTodoFileAndGetList(file);
    int indxDel = TodoFileHandler::containsTodoItemWithIdInList(list_items,id);
    std::list<TodoItem>::iterator itera = list_items.begin();
    std::advance(itera,indxDel);
    TodoItem item = *itera;
    return item;
}

void deleteSingleTodoFromSpecificFile(int id, string file){
    list<TodoItem> list_items = TodoFileHandler::readTodoFileAndGetList(file);
    int indxDel = TodoFileHandler::containsTodoItemWithIdInList(list_items,id);
    std::list<TodoItem>::iterator itera = list_items.begin();
    std::advance(itera,indxDel);
    list_items.erase(itera);

    std::list<TodoItem>::iterator iter;
    for (iter = list_items.begin(); iter != list_items.end(); ++iter){
        int posNow = std::distance(list_items.begin(), iter);
        if(iter->getId() != posNow){
            iter->setId(posNow);
        }
    }

    TodoFileHandler::writeFullListToSpecificFile(list_items,file);
}

void printConfigData(){
    cout << "Files:";
    cout << " " << whiteText << "Todo file:" << clear << " " << invert << " " << primaryFile << " " << clear << " ";
    cout << " " << whiteText << "BackUp file:" << clear << " " << invert << " " << secondaryFile << " " << clear << " ";
    cout<< endl;
    cout<< endl;
    
    cout << "Lists:";
    for(size_t i = 0; i < listOfLists.size(); i++)
    {
        Identifier sing = listOfLists.at(i);
        cout << " " << sing.getColorWithEscapes() << " " << sing.getId() << ". " << sing.getTitle() << " " << clear << " ";
    }
    cout << endl;
    cout << endl;

    cout << "Priorities:";
    for(size_t i = 0; i < listOfPriorities.size(); i++)
    {
        Identifier singP = listOfPriorities.at(i);
        cout << " " << singP.getColorWithEscapes() << " " << singP.getId() << ". " << singP.getTitle() << " " << clear << " ";
    }
    cout << endl;   
}

int identifierListContainsId(std::vector<Identifier> vec,int id){
    typedef vector<Identifier> IntContainer;
    typedef IntContainer::iterator IntIterator;
    IntContainer vw;
    IntIterator i = find_if(vec.begin(), vec.end(), [&](Identifier &f) { return f.getId() == id; });

    if (i != vec.end()) {
        return i - vec.begin();
    } else {
        return -1;
    }
}

void clearScreen(){
      system("clear");
}

void drawStartLine(){
    std::cout << escape << "[37;100m--------------------------------------------------------" << clear << endl;
}

string editInEditor(string title){
    char date[11];
    time_t t = time(0);
    struct tm *tm;
    tm = gmtime(&t);
    strftime(date,sizeof(date),"%m%d%H%M%S", tm); 
    const char* homeDir = getenv("HOME");
    std::string tempFile(homeDir);
    tempFile.append("/"+std::string(".todo")+std::string(date));
    system(("echo \""+title+"\" > "+tempFile).c_str()); //Echoes the stirng into the temp file
    system(("$EDITOR "+tempFile).c_str()); //opens the file

    string outtext = title;
    ifstream infile(tempFile);
    if (infile.good())
    {
        string sLine;
        getline(infile, sLine);
        outtext=sLine;
    }
    infile.close();
    system(("rm "+tempFile).c_str());
    return outtext;
}
