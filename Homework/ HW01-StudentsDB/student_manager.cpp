/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Created On : Sat Feb 14 2026
 *  Desc: This program manages student data. i.e Add students, display students, search by id and name and show frequency of majors
 *  File : student_manager.cpp
 *******************************************/

#include "Student.cpp"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

void displayPrompt();

int getOption();
int getPostNum();
string getWholeString();
int getMajorIndex(const vector<Student::Major>& majors, string major);

void addStudent(vector<Student>& studentsArr);
void displayStudents(const vector<Student>& studentsArr);
void searchByID(const vector<Student>& studentsArr);
void searchByName(const vector<Student>& studentsArr);
void countStudentsByMajor(const vector<Student>& studentsArr);

int main(){
    enum class Options {AddStudent = 1, DisplayStudents, SearchByID, SearchByName, CountStudentsByMajor, Exit};
    Options option;
    vector<Student> studentsArr;

    displayPrompt();
    option = (Options) getOption();  


    while(option != Options::Exit){
        switch (option) {
            case Options::AddStudent: addStudent(studentsArr); break;
            case Options::DisplayStudents: displayStudents(studentsArr); break;
            case Options::SearchByID: searchByID(studentsArr); break;
            case Options::SearchByName: searchByName(studentsArr); break;
            case Options::CountStudentsByMajor: countStudentsByMajor(studentsArr); break;
        }

        displayPrompt();
        option = (Options) getOption(); 
    }

    cout << "\n\n ---- Take care :) -----";

    return 0;
}

void displayPrompt(){
    cout << "\n Hey there! What would you like to do?" << endl
         << "1. Add student " << endl
         << "2. Display all students " << endl
         << "3. Search by ID " << endl
         << "4. Search by name " << endl
         << "5. Count students by major " << endl
         << "6. Exit \n" << endl;
    
    cout << "Pick an option: ";
}

int getOption(){
    int option;
    cin >> option; cin.ignore(10000, '\n');

    while(option < 1 || option > 6){
        cout << "Your option " << option << " is too " << (option < 1 ? "low" : "high") << endl;
        cout << "It has to be in the range 1 to 6. Try again: ";

        cin >> option; cin.ignore(10000, '\n');
    }

    return option;
}

int getPostNum(){
    int posNum;
    cin >> posNum; cin.ignore(10000, '\n');

    while(posNum < 0){
        cout << "Your number " << posNum << " is a NEGATIVE number";
        cout << "It has to be POSITIVE. Try again: ";

        cin >> posNum; cin.ignore(10000, '\n');
    }

    return posNum;
}

string getWholeString(){
    string str;
    getline(cin, str);

    return str;
}

void addStudent(vector<Student>& studentsArr){
    Student student;

    cout << "\nEnter student data \n\n";

    cout << "ID: ";
    student.setID(getPostNum());

    cout << "Name: ";
    student.setName(getWholeString());

    cout << "Age: ";
    student.setAge(getPostNum());

    cout << "Major: ";
    student.setMajor(getWholeString());

    cout << "Email: ";
    student.setEmail(getWholeString());

    studentsArr.push_back(student);

}

void displayStudents(const vector<Student>& studentsArr){
    if(studentsArr.empty()){
        cout << "\n\a\t---There are no students---\n";
        return;
    }
    cout << format("{:<7} {:<25} {:<4} {:<10}", "ID", "Name", "Age", "Major", "Email") << endl;
    cout << format("{:->55}", "") << endl;
    
    for(Student student : studentsArr){
        cout << format("{:<7} {:<25} {:<4} {:<10}", student.getID(), student.getName(), student.getAge(), student.getMajor(), student.getEmail()) << endl;
    }
}

void searchByID(const vector<Student>& studentsArr){
    int studentID;
    cout << "Student ID: ";
    studentID = getPostNum();

    for(Student student: studentsArr){
        if(student.getID() == studentID){
            cout << student.toString();
            return;
        }
    }

    cout << "\t\aStudent not found \n";
}

void searchByName(const vector<Student>& studentsArr){
    string studentName;
    cout << "Student Name: ";
    studentName = getWholeString();

    for(Student student: studentsArr){
        if(student.getName() == studentName){
            cout << student.toString();
            return;
        }
    }

    cout << "\t\aStudent not found \n";
}


void countStudentsByMajor(const vector<Student>& studentsArr){
  
    vector<Student::Major> majors;

    for(Student student : studentsArr){
        string major = student.getMajor();
        int majorIndex = getMajorIndex(majors, major);

        if(majorIndex == -1){ // checking if major is not in majors array
            majors.push_back(Student::Major{major, 1});
        }else{
            majors[majorIndex].freq++;
        }
    }

    for(Student::Major major : majors){
        cout << major.major << ": " << major.freq << endl;
    }
}

int getMajorIndex(const vector<Student::Major>& majors, string major){
    for(int i = 0; i < majors.size(); i++){
        if(majors[i].major == major)
            return i;
    }
    return -1;
}