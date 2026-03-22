#include <iterator>
#include <vector>
#include <list>
#include <iostream>
#include <string>

using namespace std;

int main(){
    vector<int> nums {10,20,30,40,50};

    cout << "In order " << endl;
    // ++it is faster that it++ since ++it is a pointer move but it++ returns an object which makes a copy
    for(vector<int>::iterator it = nums.begin(); it != nums.end(); ++it)
        cout << *it << endl;
    
    cout << "\n\n In reverse order" << endl;
    
    for(vector<int>::reverse_iterator it = nums.rbegin(); it != nums.rend(); ++it)
        cout << *it << endl;


    list<string> fruits {"Apple", "Banana", "Cherry", "Date"};

    for(list<string>::iterator it = fruits.begin(); it != fruits.end(); ++it)
        cout << *it << endl;
    
    cout << endl;
    
    back_inserter(fruits) = "Elderberry";
    
    for(list<string>::iterator it = fruits.begin(); it != fruits.end(); ++it)
        cout << *it << endl;
    
}