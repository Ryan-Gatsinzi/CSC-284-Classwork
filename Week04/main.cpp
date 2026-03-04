#include <iostream>
#include "SpreadSheetCell.cpp"
#include "SpreadSheetCell.h"

using namespace std;

int main(){
    SpreadSheetCell cell0(10), cell1;

    cout << "Cell instatiated with value = 10: " << cell0.getData() << endl;
    cout << "Cell instatiated with no value: " << cell1.getData() << endl << endl;

    cell0.setString("hello");
    cout << "set string to hello, data: " << cell0.getData();
}