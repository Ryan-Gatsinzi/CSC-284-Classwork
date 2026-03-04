#ifndef BASE_H
#define BASE_H

#include <string>

class Base {
    public:
        virtual std::string someMethod();
    
    private:
        int m_protectedInt {0};
    
    protected:
        int m_privateInt {0};
};

#endif