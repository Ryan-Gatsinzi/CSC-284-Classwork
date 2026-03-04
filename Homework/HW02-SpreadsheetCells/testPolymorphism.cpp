/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Wed March 03 2026
 *  Desc: Tests polymorphism of Double and String spread sheet cells
 *  File : testPolymorphism.cpp
 *******************************************/


#include <iostream>
#include <memory>
#include <vector>
#include "SpreadSheetCell.h"
#include "StringSpreadSheetCell.h"
#include "DoubleSpreadSheetCell.h"

using namespace std;

int main(){
    vector<unique_ptr<SpreadSheetCell>> cells;

    cells.push_back(make_unique<StringSpreadSheetCell>(StringSpreadSheetCell()));
    cells.push_back(make_unique<StringSpreadSheetCell>(StringSpreadSheetCell()));
    
    cells.push_back(make_unique<DoubleSpreadSheetCell>(DoubleSpreadSheetCell()));
    cells.push_back(make_unique<DoubleSpreadSheetCell>(DoubleSpreadSheetCell()));

    cells[0]->set("hello");
    cells[1]->set("10");

    //callng set(string_view value)
    cells[2]->set("10");
    
    //calling set(double value)
    dynamic_cast<DoubleSpreadSheetCell*>(cells[3].get())->set(19);

    cout << "Vector: [";
    for(int i = 0; i < cells.size(); i++){
        cout << cells[i]->getString() << ((i < cells.size() - 1) ? ", " : "");
    }
    cout << "]";


}