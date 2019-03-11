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

#include <time.h>
#include <stdlib.h>
//cd:rie:p:l:t:hms
#define USAGE ""\
  "Usage: todo [-h help] [-i info] [-l list] [-p priority] [-e edit] [-t tag] 'YOUR TODO THING'\n\n" \
  "  todo without any parameters will simply print all of your current todo items\n\n"\
  "  -h: Displays this message\n\n"\
  "  -i: info: Displays to the user all the current lists available with their \n"\
  "associated id's and colors. It also displays to the user all the current priorities\n"\
  "to the user with id associated id's and colors.\n\n"\
  "  -l: list: If this parameter is assigned along with user input (a todo item) then\n"\
  "the item will be allocated to this list, if however there is not user input or todo\n"\
  "then the program will simply print out ONLY specified list of todo items.\n\n"\
  "  EXAMPLE:\n"\
  "     todo -l 2 'Dont forget to water the plants'\n"\
  "     todo -l 2\n\n"\
  "  -w: Terminal width by default\n"\
  "  -l: Loops are only useful with GIF files. A value of 1 means that the GIF will "\
  "be displayed twice because it loops once. A negative value means infinite "\
  "be displayed twice because it loops once. A negative value means infinite "\
  "looping\n"\
  "  -r: Resolution must be 1 or 2. By default catimg checks for unicode support to "\
  "use higher resolution\n"\
  "  -c: Convert colors to a restricted palette\n"

using namespace std;

bool isThereText(char *argv[], int argc, string thisOpt);
void readAllTodos();
void readTodosInSingleList(int listNum);
void addATodo(string thing);
void deleteSingleTodo(int id);
void editSingleTodo(int id);
void clearAllTodos();
void clearScreen();
void drawStartLine();
void printConfigData();
int identifierListContainsId(std::vector<Identifier> vec,int id);

void experiment();

static string escape = "\u001b";
static string clear = "\u001b[0m";
static string invert = "\u001b[7m";
static string whiteAndBlack = "\u001b[30;47m";
static string whiteText = "\u001b[97m";

int currentPriority = 2;
int currentListToPutIn = 1;
string currentTag = "EMPTY";

vector<Identifier> listOfLists;
vector<Identifier> listOfPriorities;
int tagColorFg = 0;
int tagColorBg = 0;
string primaryFile = ".todo";
string secondaryFile = ".todoBacklog";
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
	primaryFile = cnfgReader->getConfigValueForKey("todoFile");
	secondaryFile = cnfgReader->getConfigValueForKey("todoBackUpFile");
	
    TodoFileHandler::setPrimaryAndSecondaryFile(primaryFile,secondaryFile);
    // cout << "Args: " << argc << endl;
    // cout << argv[0] << argc << endl;

    if(argc == 1){
        //printConfigData();
        readAllTodos();
    } else {
        int c;
        while ((c = getopt (argc, argv, "cd:rie:p:l:t:hms")) != -1){
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
                    //TODO
                    // printf("%s",optarg);
                    editSingleTodo(stoi(optarg));   
                    exit(0);
                    break;
                case 's':
                    //TODO sync the todo file
                    exit(0);
                    break;
                case 'p':
                    currentPriority = stoi(optarg);
                    break;
                case 'l':
                    currentListToPutIn = stoi(optarg);
					if(!isThereText(argv,argc,optarg)){
						readTodosInSingleList(stoi(optarg));
						exit(0);
					}
					//If no text to add then just print this list
                    break;
                case 't':
                    currentTag = optarg;
                    break;
                case 'h':
                    printf(USAGE);  
                    exit(0);
                    break;
                case 'm':
                    experiment();
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
	//itera->getTitle()  = is the string
	//could put it into a temp file
	//open file in EDITOR
	//save and close the file
	//cat file to variable
	//delete file
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
	//TODO put deleted ones into own list and write that to the backup
	//just append it
    readAllTodos();
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

void experiment(){
	cout << "This function is not ready yet!" << endl;
    string example = "This is my example string";

    char date[11];
    time_t t = time(0);
    struct tm *tm;
    tm = gmtime(&t);
    strftime(date,sizeof(date),"%m%d%H%M%S", tm); 
    // printf("%s\n",date);
    string tempFile = std::string("$HOME/")+std::string(".todo")+std::string(date);
    system(("echo \""+example+"\" > "+tempFile).c_str()); //Echoes the stirng into the temp file
    system(("$EDITOR "+tempFile).c_str()); //opens the file


    printf("nasdosdssdfdsfdsds___   dfsdf adoi");  

    std::ifstream file(tempFile);
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) { 
            printf("nasdosadoi");  
            cout << line << endl;
        }
        file.close();
    }  

    // std::ofstream outfile;

    // outfile.open(TodoFileHandler::getConfigFullFileLocation().c_str(), std::ios_base::app);
    // std::string fullRow = std::to_string(TodoFileHandler::getNumberOfItems())+""+seperator+""+tag+""+seperator+""+thing+""+seperator+""+std::to_string(pri)+""+seperator+""+std::to_string(list)+"\n";
    // outfile << fullRow;
}
