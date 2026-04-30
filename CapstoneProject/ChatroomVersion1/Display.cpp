#ifndef DISPLAY_CPP
#define DISPLAY_CPP

#include<string>
#include<vector>

class Display {
    // static int numWindows = 0;
    //int id = numWindows -> in constructor
    private:
        int height, width, scroll, id, highlight;
        std::vector<std::string> data;
    
    public:
        Display(int id) : id(id) { height = width = scroll = highlight = 0; }

        Display(int id, int height, int width, std::vector<std::string> data = {}) : id(id), height(height), width(width), data(std::move(data)), scroll(0), highlight(0) {};
        
        int getHeight(){ return height; }

        int getWidth(){ return width; }
        
        int getCount(){ return data.size(); }
        
        int getScroll(){ return scroll; }

        int getId(){ return id; }

        int getHighlight(){ return highlight; }

        std::vector<std::string>& getData(){ return data; }

        void setHeight(int height){ this->height = height; }

        void setWidth(int width){ this->width = width; }

        void setScroll(int scroll){ this->scroll = scroll; }

        void setHighlight(int highlight){ this->highlight = highlight; }
};

#endif