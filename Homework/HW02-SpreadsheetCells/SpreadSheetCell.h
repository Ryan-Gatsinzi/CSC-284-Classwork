/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Wed March 03 2026
 *  Desc: Definition of the SpreadSheetCell abstract base class
 *  File : SpreadSheetCell.cpp
 *******************************************/

#ifndef SPREADSHEETCELL_H
#define SPREADSHEETCELL_H

#include <string>
#include <string_view>

class SpreadSheetCell {
    public:
        virtual ~SpreadSheetCell() = default;
        virtual void set(std::string_view value) = 0;        
        virtual std::string getString() const = 0;
};

#endif