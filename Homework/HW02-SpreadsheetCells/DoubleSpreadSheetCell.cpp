/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Wed March 03 2026
 *  Desc: Implementation of the DoubleSpreadSheetCell derived class
 *  File : DoubleSpreadSheetCell.cpp
 *******************************************/

#ifndef DOUBLESPREADSHEETCELL_CPP
#define DOUBLESPREADSHEETCELL_CPP


#include "DoubleSpreadSheetCell.h"
#include<string>
#include<string_view>

using namespace std;

    void DoubleSpreadSheetCell::set(double value){
        this->value = value;
    }
    
    void DoubleSpreadSheetCell::set(string_view value){
        this->value = stringToDouble(value);
    }

    string DoubleSpreadSheetCell::getString() const{
        return value.has_value() ? doubleToString(value.value()) : "";
    }

    string DoubleSpreadSheetCell::doubleToString(double value) const {
        return to_string(value);
    }

    double DoubleSpreadSheetCell::stringToDouble(string_view value){
        return stod(value.data());
    }



#endif