#include <iostream>
using namespace std;

int main(){
    int max_num;

    cout << "Give me a max number: ";
    cin >> max_num; cin.ignore();

    for(int i {1}; i <= max_num; i++){
        if(i % 3 != 0 && i % 5 != 0) 
            cout << i << endl;
        else
            cout << (i % 3 == 0 ? " FIZZ " : "") << (i % 5 == 0 ? "BUZZ " : "") << endl;
    }
}