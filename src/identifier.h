#pragma once
#include <string>

class Identifier{
    private:
        int m_id;
        std::string m_label;
        int m_foreground;
        int m_background;
    public:
        Identifier(int id, std::string label, int foreground, int background) : m_id(id), m_label(label), m_foreground(foreground), m_background(background){};
        void printOut();
        int getId(){
            return m_id;
        };
        std::string getTitle(){
            return m_label;
        };
        int getForeground(){
            return m_foreground;
        };
        int getBackground(){
            return m_background;
        };
};