/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Wed March 03 2026
 *  Desc: Implementation of the StringSpreadSheetCell derived class
 *  File : StringSpreadSheetCell.cpp
 *******************************************/

#ifndef STRINGSPREADSHEETCELL_CPP
#define STRINGSPREADSHEETCELL_CPP

#include "StringSpreadSheetCell.h"

using namespace std;


    void StringSpreadSheetCell::set(string_view value){
        this->value = value.data();
    }

    string StringSpreadSheetCell::getString() const{
        return value.value_or("");
    }


#endif