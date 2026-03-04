#include <iostream>
#include "derived.h"


using namespace std;


int main(){
   Derived* myDerived = new Base();
   Base& ref = myDerived;

   cout << ref.someMethod()<<endl;
}