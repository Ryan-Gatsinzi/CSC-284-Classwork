#ifndef DERIVED_H
#define DERIVED_H

#include <string>
#include "base.h"

class Derived : public Base {
    public:
        std::string someMethod() override;
        std::string someOtherMethod();
};

#endif