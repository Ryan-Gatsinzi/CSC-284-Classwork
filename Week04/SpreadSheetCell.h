#ifndef SPREADSHEETCELL_H
#define SPREADSHEETCELL_H

#include <string>

class SpreadSheetCell {
    private:
        double data;
    
    public:
        SpreadSheetCell();
        SpreadSheetCell(double data);

        double getData() const;
        
        void setData(double data);

        void setString(std::string_view str);
        

};

#endif