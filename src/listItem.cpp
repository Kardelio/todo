#include "listItem.h"
#include <vector>

void ListItem::addTodoItemToList(TodoItem item){
    listOfTodoItems.push_back(item);
}

std::vector<TodoItem> ListItem::getListOfTodoItems(){
    return listOfTodoItems;
}

std::string ListItem::getTitle(){
    return title;
}

int ListItem::getId(){
    return listId;
}