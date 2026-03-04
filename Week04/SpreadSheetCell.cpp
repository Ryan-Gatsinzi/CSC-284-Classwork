#ifndef SPREADSHEETCELL_CPP
#define SPREADSHEETCELL_CPP

#include "SpreadSheetCell.h"

SpreadSheetCell::SpreadSheetCell(double data) {
    this->data = data;
}
SpreadSheetCell::SpreadSheetCell() : SpreadSheetCell::SpreadSheetCell(0){}

double SpreadSheetCell::getData() const {
    return data;
}

void SpreadSheetCell::setData(double data) {
    this->data = data;
}

void SpreadSheetCell::setString(std::string_view str){
    data = std::strtod(str.data(), nullptr);
}

#endif