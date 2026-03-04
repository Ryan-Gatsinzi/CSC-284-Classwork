#ifndef BANKACCOUNT_CPP
#define BANKACCOUNT_CPP

#include <string>
using namespace std;

class BankAccount{
    private:
        string accountHolder;
        int accountNumber;
        double balance;
    
    public:
        static int totalAccounts, nextAccountNumber;

        BankAccount(string accountHolder, double balance)
};

#endif