/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Wed March 03 2026
 *  Desc: Definition of the StringSpreadSheetCell derived class
 *  File : StringSpreadSheetCell.h
 *******************************************/


#ifndef STRINGSPREADSHEETCELL_H
#define STRINGSPREADSHEETCELL_H

#include "SpreadSheetCell.h"
#include <optional>
#include<string>
#include<string_view>

using namespace std;

class StringSpreadSheetCell : public SpreadSheetCell {
    private:
        optional<string> value;
    
    public:
        void set(string_view value) override;
        string getString() const override;

};

#endif