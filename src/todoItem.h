#pragma once
#include <string>
#include <vector>
#include "configReader.h"

class TodoItem{
    private:
        int m_id;
        int m_listId;
        std::string m_tag;
        std::string m_title;
        int m_priority;
        const std::string escape = "\u001b";
        const std::string clear = "\u001b[0m";
    public:
        TodoItem(int id, int listId, std::string tag, std::string title, int priority) : m_id(id), m_listId(listId), m_tag(tag), m_title(title), m_priority(priority){};
        void printItem();
        std::string getName();
        void standardPrint();
        void backlogPrint();
        void printWithFGandBG(int fg, int bg, ConfigReader cfg, std::vector<Identifier> listOfPris);
        void printWithFGandBGNew(std::string color, ConfigReader cfg, std::vector<Identifier> listOfPris);
        int getId();
        void setId(int idIn);
        int getListId(){
            return m_listId;
        }
        std::string getTag(){
            return m_tag;
        }
        std::string getTitle(){
            return m_title;
        }
        int getPriority(){
            return m_priority;
        }
        void setListId(int idin){
            m_listId = idin;
        }
        void setTag(std::string tagIn){
            m_tag = tagIn;
        }
        void setTitle(std::string titleIn){
            m_title = titleIn;
        }
        void setPriority(int priorityIn){
            m_priority = priorityIn;
        }
};
