/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Wed March 03 2026
 *  Desc: Definition of the DoubleSpreadSheetCell derived class
 *  File : DoubleSpreadSheetCell.h
 *******************************************/

#ifndef DOUBLESPREADSHEETCELL_H
#define DOUBLESPREADSHEETCELL_H

#include "SpreadSheetCell.h"
#include <optional>
#include<string>
#include<string_view>

using namespace std;

class DoubleSpreadSheetCell : public SpreadSheetCell {
    private:
        optional<double> value;
    
    public:
        void set(double value);
        void set(string_view value) override;
        string getString() const override;
        string doubleToString(double value) const;
        double stringToDouble(string_view value);

};

#endif