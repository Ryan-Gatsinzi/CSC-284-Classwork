#include <iostream>
#include <vector>
#include <concepts>
#include <string>

using namespace std;

template<typename X>
concept HasSize = requires(X a){
    a.size();
};

template<HasSize T>
void printSize(const T& container){
    cout << container.size() << endl;
}   


int main(){
   vector<int> v{1, 2, 3};
   string s = "hello";

   printSize(v);
   printSize(s);

   int x = 10;
   printSize(x);
}