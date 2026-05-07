#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <regex>

using namespace std;


void equationSolver(istream& is){
    string eqn;

    char operators[4] = {'+','-','*','/'};

    // regex addPattern(R"((\d{1}) + (\d{1}))");
    // regex subtractPattern(R"((\d{1}) - (\d{1}))");
    // regex dividePattern(R"((\d{1}) / (\d{1}))");

    vector<regex> patterns = {regex(R"((\d{1}) + (\d{1}))"), regex(R"((\d{1}) - (\d{1}))"), regex(R"((\d{1}) / (\d{1}))")};
    smatch match;


    while(getline(is, eqn)){
        int operand_index;

        for(int i = 0; i < patterns.size(); i++){
            if(regex_match(eqn, match, patterns[i]))
                operand_index = i;
        }

        int op1 = stoi(match[1]);
        int op2 = stoi(match[2]);

        switch(operand_index){
            case 0:
                cout << match[1] << " + " << match[2] << " = " << op1 + op2;
                break;
            case 1:
                cout << match[1] << " + " << match[2] << " = " << op1 - op2;
                break;
            case 2:
                cout << match[1] << " * " << match[2] << " = " << op1 * op2;
                break;
            case 3:
                cout << match[1] << " / " << match[2] << " = " << op1 * op2;
                break;
        }
    }
}

int main(){
    string filename;
    ifstream inputFileStream;

    cout << "What file would you like to read operations from: ";
    getline(cin, filename);
    
    inputFileStream.open(filename);
    
    while(!inputFileStream){
        cout << "Failed to open file " << filename << ". Try again: ";
        getline(cin, filename);

        inputFileStream.open(filename);
    }

    equationSolver(inputFileStream);

}
