#include <iostream>
#include <string>
using namespace std;

template<typename T>
void printValue(T val){
    cout << val << endl;
}   

// template<>
// void printValue<bool>(bool value){
//     cout << (value ? "True" : "false") << endl;
// }

// void printValue(bool value){
//     cout << (value ? "True" : "false") << endl;
// }

int main(){
    printValue(43);
    printValue(3.14);
    printValue(true);
    printValue(false);
}