#include "todoFileHandler.h"
#include "stringUtils.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <list>

using namespace std;

TodoFileHandler::TodoFileHandler(){
}

//TODO make this file the primary file on the config
std::string TodoFileHandler::todoFileName = ".todo";
std::string TodoFileHandler::secondaryTodoFileName = ".todoBacklog";
std::string TodoFileHandler::seperator = "+++";
int TodoFileHandler::numberOfItems = 0;

std::string TodoFileHandler::getConfigFullFileLocation(){
    const char* homeDir = getenv("HOME");
    std::string s(homeDir);
    s.append("/"+todoFileName);
    return s;
}

void TodoFileHandler::setPrimaryAndSecondaryFile(std::string prim, std::string second){
	TodoFileHandler::todoFileName = prim;
	TodoFileHandler::secondaryTodoFileName = second;
}

void TodoFileHandler::readTempFile(){
    list<TodoItem> listOfItems;
    std::ifstream file(TodoFileHandler::getConfigFullFileLocation().c_str());
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            vector<string> a = splitStringIntoArrayUsingDelim(line,seperator);
            int ida = stoi(a.at(0));
            int pria = stoi(a.at(3));
            int listid = stoi(a.at(4));
            TodoItem item(ida, listid, a.at(1), a.at(2), pria);
            listOfItems.push_back(item);
        }
        file.close();
    }
}

std::vector<TodoItem> TodoFileHandler::readTodoFileAndGetVector(){
    vector<TodoItem> listOfItems;
    std::ifstream file(TodoFileHandler::getConfigFullFileLocation().c_str());
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            vector<string> a = splitStringIntoArrayUsingDelim(line,seperator);
            int ida = stoi(a.at(0));
            int pria = stoi(a.at(3));
            int listid = stoi(a.at(4));
            TodoItem item(ida, listid, a.at(1), a.at(2), pria);
            listOfItems.push_back(item);
        }
        file.close();
    }
    return listOfItems;
}

std::list<TodoItem> TodoFileHandler::readTodoFileAndGetList(){
    list<TodoItem> listOfItems;
    std::ifstream file(TodoFileHandler::getConfigFullFileLocation().c_str());
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            vector<string> a = splitStringIntoArrayUsingDelim(line,seperator);
            int ida = stoi(a.at(0));
            int pria = stoi(a.at(3));
            int listid = stoi(a.at(4));
            TodoItem item(ida, listid, a.at(1), a.at(2), pria);
            listOfItems.push_back(item);
        }
        file.close();
    }
    return listOfItems;
}

int TodoFileHandler::getNumberOfItems(){
    int counter = 0;
    std::ifstream file(TodoFileHandler::getConfigFullFileLocation().c_str());
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            counter++;
        }
        file.close();
    }
    numberOfItems = counter;
    return numberOfItems;
}

void TodoFileHandler::addTodoItemToFile(int list, std::string tag, std::string thing, int pri){
    std::ofstream outfile;

    outfile.open(TodoFileHandler::getConfigFullFileLocation().c_str(), std::ios_base::app);
    std::string fullRow = std::to_string(TodoFileHandler::getNumberOfItems())+""+seperator+""+tag+""+seperator+""+thing+""+seperator+""+std::to_string(pri)+""+seperator+""+std::to_string(list)+"\n";
    outfile << fullRow;
}

std::vector<ListItem> TodoFileHandler::readTodoFileIntoListItems(){
    vector<ListItem> listOfLists;
    std::ifstream file(TodoFileHandler::getConfigFullFileLocation().c_str());
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            vector<string> a = splitStringIntoArrayUsingDelim(line,seperator);
            int ida = stoi(a.at(0));
            int pria = stoi(a.at(3));
            int listid = stoi(a.at(4));
            TodoItem item(ida, listid, a.at(1), a.at(2), pria);
            int indexOfOtherList = containsListItemWithId(listOfLists,listid);
            if(indexOfOtherList > -1){
                listOfLists.at(indexOfOtherList).addTodoItemToList(item);
            } else {
                listOfLists.push_back(ListItem(listid,{item}));
            }
        }
        file.close();
    }
    return listOfLists;
}

std::vector<ListItem> TodoFileHandler::readTodoFileIntoListItemsWithSingleList(int listNum){
    vector<ListItem> listOfLists;
    std::ifstream file(TodoFileHandler::getConfigFullFileLocation().c_str());
    if (file.is_open()) {
        std::string line;
        while (getline(file, line)) {
            vector<string> a = splitStringIntoArrayUsingDelim(line,seperator);
            int listid = stoi(a.at(4));
			if(listid == listNum){
				int ida = stoi(a.at(0));
				int pria = stoi(a.at(3));
				TodoItem item(ida, listid, a.at(1), a.at(2), pria);
				int indexOfOtherList = containsListItemWithId(listOfLists,listid);
				if(indexOfOtherList > -1){
					listOfLists.at(indexOfOtherList).addTodoItemToList(item);
				} else {
					listOfLists.push_back(ListItem(listid,{item}));
				}
			}
        }
        file.close();
    }
    return listOfLists;
}

int TodoFileHandler::containsTodoItemWithId(std::vector<TodoItem> vec,int id){
    typedef vector<TodoItem> IntContainer;
    typedef IntContainer::iterator IntIterator;
    IntContainer vw;
    IntIterator i = find_if(vec.begin(), vec.end(), [&](TodoItem &f) { return f.getId() == id; });

    if (i != vec.end()) {
        return i - vec.begin();
    } else {
        return -1;
    }
}

int TodoFileHandler::containsTodoItemWithIdInList(std::list<TodoItem> vec,int id){
    list<TodoItem>::iterator it;
	it = std::find_if(vec.begin(), vec.end(), [&](TodoItem &f) { return f.getId() == id; });
 
    if (it != vec.end()) {
        return std::distance(vec.begin(), it);
    } else {
        return -1;
    }
}

int TodoFileHandler::containsListItemWithId(std::vector<ListItem> vec,int id){
    typedef vector<ListItem> IntContainer;
    typedef IntContainer::iterator IntIterator;
    IntContainer vw;
    IntIterator i = find_if(vec.begin(), vec.end(), [&](ListItem &f) { return f.getId() == id; });

    if (i != vec.end()) {
        return i - vec.begin();
    } else {
        return -1;
    }
}

void TodoFileHandler::writeFullListToFile(std::list<TodoItem> list_of_items){
    ofstream ofs;
    ofs.open(TodoFileHandler::getConfigFullFileLocation().c_str(), std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    std::list<TodoItem>::iterator iter;
    for (iter = list_of_items.begin(); iter != list_of_items.end(); ++iter){
        addTodoItemToFile(iter->getListId(),iter->getTag(),iter->getTitle(),iter->getPriority());
    }
}
