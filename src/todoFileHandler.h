#pragma once
#include <vector>
#include <list>
#include "todoItem.h"
#include "listItem.h"

class TodoFileHandler{
    private:
        static std::string todoFileName;
        static std::string secondaryTodoFileName;
        static std::string seperator;
        static std::string getConfigFullFileLocation();
        static int numberOfItems;
    public:
        TodoFileHandler();
        static void setPrimaryAndSecondaryFile(std::string prim, std::string second);
        static std::vector<TodoItem> readTodoFileAndGetVector();
        static std::list<TodoItem> readTodoFileAndGetList();
        static int getNumberOfItems();
        static void addTodoItemToFile(int list, std::string tag, std::string thing, int pri);
        static std::vector<ListItem> readTodoFileIntoListItems();
        static std::vector<ListItem> readTodoFileIntoListItemsWithSingleList(int listNum);
        static int containsTodoItemWithId(std::vector<TodoItem> vec,int id);
        static int containsListItemWithId(std::vector<ListItem> vec,int id);
        static int containsTodoItemWithIdInList(std::list<TodoItem> vec,int id);
        static void writeFullListToFile(std::list<TodoItem> list_of_items);
        static void readTempFile();
};
