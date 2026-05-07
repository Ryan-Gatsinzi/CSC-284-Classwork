#include <iostream>
#include <string>

using namespace std;

string shift(string encrpt, int step){
    for(int i = 0; i < encrpt.length(); i++){

        if(isalpha(encrpt[i])){
            int min =  (encrpt[i] > 'Z' ? 'a' : 'A');
            int max = (encrpt[i] < 'a' ? 'Z' : 'z');

            int new_pos = (int)encrpt[i] - step;
            int over_bound = (new_pos < min ? step - ((int)encrpt[i] - min) : 0);
           
            int new_ascii = (over_bound == 0 ? new_pos : max - (over_bound - 1));
            encrpt[i] = new_ascii;
        }
    }

    return encrpt;
}

int main(){
    string encrypted_text;

    cout << "Encrypted text: ";
    getline(cin, encrypted_text);
    
    for(int i = 0; i < 26; i++)
        cout <<  "Shifted by: " << i << "; text: " << shift(encrypted_text, i) << endl;



}