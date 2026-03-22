#include <iostream>
#include <string>

using namespace std;


void swapInt(int& i1, int& i2){
    int temp {i2};
    i2 = i1;
    i1 = temp;
}

void swapDouble(double& d1, double& d2){
    double temp {d2};
    d2 = d1;
    d1 = temp;
}

void swapString(string& s1, string& s2){
    string temp {s2};
    s2 = s1;
    s1 = temp;
}


template<typename T>
void mySwap(T& elem1, T& elem2){
    T temp = elem2;
    elem2 = elem1;
    elem1 = temp;    
}
int main(){
    int a = 1, b = 9;
    double c = 3.98, d = 0.33;
    string e = "hello world", f = "Jesus saves";

    cout << "a: " << a << endl
        << "b: " << b << endl
        << "c: " << c << endl
        << "d: " << d << endl
        << "e: " << e << endl
        << "f: " << f << endl; 

    cout << endl << "calling swap" << endl;

    mySwap(a, b);
    mySwap(c, d);
    mySwap(e, f);

    cout << "a: " << a << endl
        << "b: " << b << endl
        << "c: " << c << endl
        << "d: " << d << endl
        << "e: " << e << endl
        << "f: " << f << endl; 

}