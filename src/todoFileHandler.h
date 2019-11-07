#pragma once
#include <vector>
#include <list>
#include "todoItem.h"
#include "listItem.h"

class TodoFileHandler{
    private:
        static std::string seperator;
        static int numberOfItems;
    public:
        static std::string todoFileName;
        static std::string secondaryTodoFileName;
        TodoFileHandler();
        static std::string getSpecificConfig(std::string file);
        static void setPrimaryAndSecondaryFile(std::string prim, std::string second);
        static std::list<TodoItem> readTodoFileAndGetList(std::string fileToUse);
        static int getNumberOfItemsInSpecificFile(std::string fileToCheck);
        static void addactualTDToSpecificFile(TodoItem item, std::string fileToWriteTo);
        static void addTodoItemToFileToSpecificFile(int list, std::string tag, std::string thing, int pri, std::string fileToWriteTo);
        static std::vector<ListItem> readTodoFileIntoListItems();
        static std::vector<ListItem> readTodoFileIntoListItemsWithSingleList(int listNum);
        static std::vector<ListItem> readTodoFileIntoListItemsWithSinglePriority(int priNum);
        static std::vector<TodoItem> readBackLogFileIntoListItems();
        static int containsTodoItemWithId(std::vector<TodoItem> vec,int id);
        static int containsListItemWithId(std::vector<ListItem> vec,int id);
        static int containsTodoItemWithIdInList(std::list<TodoItem> vec,int id);
        static void writeFullListToSpecificFile(std::list<TodoItem> list_of_items, std::string fileToUse);
        static void readTempFile();
};
