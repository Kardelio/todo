#include "todoItem.h"
#include "todoFileHandler.h"
#include "configReader.h"
#include <algorithm>
#include <iostream>

//Constructor done in header file

void TodoItem::printItem(){
    std::cout << "Item: " << m_id << " = " << m_title << std::endl;
}

std::string TodoItem::getName(){
    return m_title;
}

void TodoItem::backlogPrint(){
    std::cout << m_id << " - " << m_title << std::endl;
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
    std::string priStr = i.getColorWithEscapes()+" "+i.getTitle()+" "+clear+" "; //was bold

    std::cout << escape << "[" << fg << ";" << bg << ";1" << "m" << " " << m_id << " " << clear  << ": " << priStr << m_title << " " << tagStr << std::endl;
}

void TodoItem::printWithFGandBGNew(std::string color, ConfigReader cfg, std::vector<Identifier> listOfPris){
    std::string tagStr = "";
    if(m_tag != "EMPTY"){
        tagStr = escape+"["+cfg.getConfigValueForKey("tagColorFront")+";"+cfg.getConfigValueForKey("tagColorBack")+";1m "+m_tag+" "+clear+" ";
    }
    //    const auto eq_id_num = std::find_if(listOfPris.begin(),listOfPris.end(),[](const Identifier & m) -> bool { return m.getId() == m_priority});
    //auto it = find_if(listOfPris.begin(), listOfPris.end(),[&m_priority](const Identifier& ide) {return ide.getId() == m_priority;});
    //std::cout << "-" << m_priority;
    //auto it = std::find_if(listOfPris.begin(), listOfPris.end(),[&](const Identifier &ide) {return ide.getId() == m_priority-1;});
    //auto it = std::find_if(begin(listOfPris), end(listOfPris),[]( const Identifier& idef) -> bool {return idef.getId() == m_priority-1;});
    //auto it = std::find_if(begin(listOfPris), end(listOfPris),[&] (Identifier const& p) { return true;});
    auto it = std::find_if(begin(listOfPris), end(listOfPris),[&] (Identifier & p) { return p.getId() == m_priority;});

    //std::cout << "Here: " << it->getTitle();
    //Identifier i = listOfPris.at((m_priority - 1)); //This is the porblem!
    //Identifier i = listOfPris.at((m_priority - 1)); //This is the porblem!
    //std::cout << i.getId();

    std::string priStrColor = it->getColorWithEscapes()+" ";
    std::string priStr = it->getColorWithEscapes()+" "+it->getTitle()+" "+clear+" ";

    std::cout << color << " " << m_id << " " << clear  << ": " << priStrColor << m_title << " " << clear << " " << tagStr << std::endl;
    //std::string priStrColor = i.getColorWithEscapes()+" ";
    //std::string priStr = i.getColorWithEscapes()+" "+i.getTitle()+" "+clear+" ";

    //std::cout << color << " " << m_id << " " << clear  << ": " << priStrColor << m_title << " " << clear << " " << tagStr << std::endl;
}

int TodoItem::getId(){
    return m_id;
}

void TodoItem::setId(int idIn){
    m_id = idIn;
}
