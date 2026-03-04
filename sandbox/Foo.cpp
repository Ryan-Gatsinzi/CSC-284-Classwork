#include <iostream>

class Foo {
    public:
        int val;
        Foo(int val) : val(val){}

        Foo(const Foo& other){
            std::cout<< "copy constructor called";
            val = other.val;
        }
        Foo(Foo&& other){
            std::cout<< "move constructor called";
        }
};

void bait(Foo foo){
    
}
int main(){
    bait(Foo(1));
    
    return 0;
}