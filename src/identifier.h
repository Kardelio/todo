#pragma once
#include <string>

class Identifier{
    private:
        int m_id;
        std::string m_label;
        std::string m_color;
    public:
        Identifier(int id, std::string label, std::string color) : m_id(id), m_label(label), m_color(color) {};
        int getId(){
            return m_id;
        };
        std::string getTitle(){
            return m_label;
        };
        std::string getColor(){
            return m_color;
        }
        std::string getColorWithEscapes(){
            return "\u001b["+m_color+"m";
        }
};
