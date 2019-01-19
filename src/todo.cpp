#include <algorithm>
#include <iostream>
#include "configReader.h"
#include "stringUtils.h"
#include "todoFileHandler.h"
#include "identifier.h"
#include "todoItem.h"
#include <vector>
#include <list>
#include <fstream>
#include <unistd.h>

#define USAGE "Usage: catimg [-h] [-w width] [-l loops] [-r resolution] image-file\n\n" \
  "  -h: Displays this message\n"                                      \
  "  -w: Terminal width by default\n"                           \
  "  -l: Loops are only useful with GIF files. A value of 1 means that the GIF will " \
  "be displayed twice because it loops once. A negative value means infinite " \
  "looping\n"                                                           \
  "  -r: Resolution must be 1 or 2. By default catimg checks for unicode support to " \
  "use higher resolution\n" \
  "  -c: Convert colors to a restricted palette\n"

using namespace std;

void readAllTodos();
void addATodo(string thing);
void deleteSingleTodo(int id);
void editSingleTodo(int id);
void clearAllTodos();
void printConfigData();
int identifierListContainsId(std::vector<Identifier> vec,int id);

static string escape = "\u001b";
static string clear = "\u001b[0m";

int currentPriority = 2;
int currentListToPutIn = 1;
string currentTag = "EMPTY";

vector<Identifier> listOfLists;
vector<Identifier> listOfPriorities;
int tagColorFg = 0;
int tagColorBg = 0;
ConfigReader *cnfgReader = new ConfigReader();


int main(int argc, char *argv[])
{
    // *cnfgReader = new ConfigReader();
    cnfgReader->checkForExistingConfig();
    cnfgReader->readConfigFileIn();
    listOfLists = cnfgReader->getLists();
    listOfPriorities = cnfgReader->getPriorities();
    tagColorFg = stoi(cnfgReader->getConfigValueForKey("tagColorFront"));
    tagColorBg = stoi(cnfgReader->getConfigValueForKey("tagColorBack"));

    // cout << "Args: " << argc << endl;
    // cout << argv[0] << argc << endl;

    if(argc == 1){
        printConfigData();
        readAllTodos();
    } else {
        int c;
        while ((c = getopt (argc, argv, "cd:rie:p:l:t:h")) != -1){
            switch (c) {
                case 'c':
                    clearAllTodos();
                    exit(0);
                    break;
                case 'd':
                    deleteSingleTodo(stoi(optarg));
                    exit(0);
                    break; 
                case 'r':
                    //TODO
                    exit(0);
                    break;
                case 'i':
                    printConfigData();
                    exit(0);
                    break;
                case 'e':
                    //TODO
                    // printf("%s",optarg);
                    editSingleTodo(stoi(optarg));   
                    exit(0);
                    break;
                case 'p':
                    currentPriority = stoi(optarg);
                    break;
                case 'l':
                    currentListToPutIn = stoi(optarg);
                    break;
                case 't':
                    currentTag = optarg;
                    break;
                case 'h':
                    printf(USAGE);  
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

void addATodo(string thing){
    TodoFileHandler::addTodoItemToFile(currentListToPutIn,currentTag,thing,currentPriority);
}

void readAllTodos(){
    vector<ListItem> list_items = TodoFileHandler::readTodoFileIntoListItems();
    if(list_items.size() > 0){
        for(size_t i = 0; i < list_items.size(); i++)
        {
            int res = identifierListContainsId(listOfLists,list_items.at(i).getId());
            if(res > -1){
                Identifier item = listOfLists.at(res);
                cout << endl;
                cout << escape << "[" << item.getForeground() << ";" << item.getBackground() << "m" << "    " << item.getId() << ". " << escape << "[1m" << item.getTitle() << "      " << clear << endl;
                
                for(size_t j = 0; j < list_items.at(i).getListOfTodoItems().size(); j++)
                {
                    TodoItem t = list_items.at(i).getListOfTodoItems().at(j);
                    t.printWithFGandBG(listOfLists.at(res).getForeground(),listOfLists.at(res).getBackground(), *cnfgReader,listOfPriorities);
                }
            }
        }
    } else {
        cout << "Empty" << endl;
    }
    cout << endl;
}

void clearAllTodos(){
    cout << "Are you sure you wish to CLEAR ALL the Todos? ... (y/n)(yes/no)" << endl;
    string answer;
    cin >> answer;
    if(answer == "y" || answer == "Y" || answer == "yes" || answer == "YES"){
        TodoFileHandler::writeFullListToFile({});
        readAllTodos();
    } else {
        cout << "Operation Aborted" << endl;
    }
}

void editSingleTodo(int id){
    printConfigData();

    list<TodoItem> list_items = TodoFileHandler::readTodoFileAndGetList();
    int indxFind = TodoFileHandler::containsTodoItemWithIdInList(list_items,id);
    std::list<TodoItem>::iterator itera = list_items.begin();
    std::advance(itera,indxFind);
    cout << "Currently: " << endl;
    itera->standardPrint();

    cout << escape << "[37;44m" << "Edit the NEW Title (Leave blank to keep it as it is...)" << clear << endl;
    string newTitle;
    getline(cin,newTitle);
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

    TodoFileHandler::writeFullListToFile(list_items);
    readAllTodos();
}

void deleteSingleTodo(int id){
    list<TodoItem> list_items = TodoFileHandler::readTodoFileAndGetList();
    int indxDel = TodoFileHandler::containsTodoItemWithIdInList(list_items,id);
    std::list<TodoItem>::iterator itera = list_items.begin();
    std::advance(itera,indxDel);
    cout << "Deleting : " << itera->getId() << ": "<< itera->getName() << endl;
    list_items.erase(itera);

    std::list<TodoItem>::iterator iter;
    for (iter = list_items.begin(); iter != list_items.end(); ++iter){
        int posNow = std::distance(list_items.begin(), iter);
        // cout << "" << posNow << endl;
        if(iter->getId() != posNow){
            iter->setId(posNow);
        }
    }

    TodoFileHandler::writeFullListToFile(list_items);
    readAllTodos();
}

void printConfigData(){
    cout << "Lists:";
    for(size_t i = 0; i < listOfLists.size(); i++)
    {
        Identifier sing = listOfLists.at(i);
        cout << " " << escape << "[" << sing.getForeground() << ";" << sing.getBackground() << ";1m " << sing.getId() << ". " << sing.getTitle() << " " << clear << " ";
    }
    cout << endl;

    cout << "Priorities:";
    for(size_t i = 0; i < listOfPriorities.size(); i++)
    {
        Identifier singP = listOfPriorities.at(i);
        cout << " " << escape << "[" << singP.getForeground() << ";" << singP.getBackground() << ";1m " << singP.getId() << ". " << singP.getTitle() << " " << clear << " ";
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
