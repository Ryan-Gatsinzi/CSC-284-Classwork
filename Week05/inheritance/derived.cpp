#ifndef DERIVED_CPP
#define DERIVED_CPP

#include "derived.h"
#include <string>

    std::string Derived::someMethod() {
        return "Derived::someMethod() Called!";
    }

    std::string Derived::someOtherMethod(){
        return "Derived::someOtherMethod() Called!";
    }

#endif