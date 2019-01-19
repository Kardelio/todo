#include "todoItem.h"
#include "todoFileHandler.h"
#include "configReader.h"
#include <iostream>

//Constructor done in header file

void TodoItem::printItem(){
    std::cout << "Item: " << m_id << " = " << m_title << std::endl;
}

std::string TodoItem::getName(){
    return m_title;
}

void TodoItem::standardPrint(){
    std::cout << escape << "[7m Title:" << clear << " " << m_title << std::endl;
    std::cout << escape << "[7m Tag:" << clear << " " << m_tag << std::endl;
    std::cout << escape << "[7m Priority:" << clear << " " << m_priority << std::endl;
    std::cout << escape << "[7m List Id:" << clear << " " << m_listId << std::endl; 
}

void TodoItem::printWithFGandBG(int fg, int bg, ConfigReader cfg, std::vector<Identifier> listOfPris){
    std::string tagStr = "";
    if(m_tag != "EMPTY"){
        tagStr = escape+"["+cfg.getConfigValueForKey("tagColorFront")+";"+cfg.getConfigValueForKey("tagColorBack")+";1m "+m_tag+" "+clear+" ";
    }
    Identifier i = listOfPris.at((m_priority - 1));
    std::string priStr = escape+"["+std::to_string(i.getForeground())+";"+std::to_string(i.getBackground())+";1m "+i.getTitle()+" "+clear+" ";
    
    std::cout << escape << "[" << fg << ";" << bg << ";1" << "m" << " " << m_id << " " << clear  << ": " << priStr << tagStr << m_title << std::endl;
}

int TodoItem::getId(){
    return m_id;
}

void TodoItem::setId(int idIn){
    m_id = idIn;
}