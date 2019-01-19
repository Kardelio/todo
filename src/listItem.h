#include <vector>
#include "todoItem.h"

class ListItem{
    private:
        std::vector<TodoItem> listOfTodoItems;
        int listId;
        std::string title;
    public:
        ListItem(int id, std::vector<TodoItem> list) : listId(id), listOfTodoItems(list){};
        void addTodoItemToList(TodoItem item);
        std::vector<TodoItem> getListOfTodoItems();
        std::string getTitle();
        int getId();
};