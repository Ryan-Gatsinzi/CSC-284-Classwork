/*******************
 * Author: Ryan Gatsinzi Udatinya
 * Filename: Display.cpp
 * last_updated: 07_05_2026
 *******************/

#ifndef DISPLAY_CPP
#define DISPLAY_CPP

#include <string>
#include <vector>
#include <mutex>


class Display {
    private:
        int height, width, scroll, id, highlight;
        std::vector<std::string> data;
        std::mutex displayMutex;
    
    public:
        Display(int id) : id(id) { height = width = scroll = highlight = 0; }

        Display(int id, int height, int width, std::vector<std::string> data = {}) : id(id), height(height), width(width), data(std::move(data)), scroll(0), highlight(0) {};
        
        int getHeight(){ return height; }

        int getWidth(){ return width; }
        
        int getCount(){ return data.size(); }
        
        int getScroll(){ return scroll; }

        int getId(){ return id; }

        int getHighlight(){ return highlight; }

        std::string getHighlightedItem() { return data[highlight]; }

        std::vector<std::string>& getData(){ std::lock_guard<std::mutex> lock(displayMutex); return data; }

        void setHeight(int height){ this->height = height; }

        void setWidth(int width){ this->width = width; }

        void setScroll(int scroll){ this->scroll = scroll; }

        void setHighlight(int highlight){ this->highlight = highlight; }
};

#endif