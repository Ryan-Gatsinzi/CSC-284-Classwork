/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Created On : Sat Feb 14 2026
 *  Desc: Class that describes a Student and a mini class that describes a Major
 *  File : Student.cpp
 *******************************************/


#ifndef STUDENT_CPP
#define STUDENT_CPP

#include <string>

using namespace std;

class Student {
    private:
        int id, age;
        string name, major, email;

    public:
        Student() {};
        Student(int id, string name, int age, string major, string email) : id(id), name(name), age(age), major(major), email(email) {};

        int getID(){ return id; }

        string getName(){ return name; }

        int getAge(){ return age; }

        string getMajor(){ return major; }

        string getEmail(){ return email; }

        void setID(int id){ this->id = id; }
        
        void setName(string name){ this->name = name; }

        void setAge(int age){ this->age = age; }

        void setMajor(string major){ this->major = major; }
        
        void setEmail(string email){ this->email = email; }

        string toString(){
            return "\n ID: " + to_string(id) + "\n Name: " + name + "\n Age: " + to_string(age) + "\n Major: " + major + "\n Email: " + email + "\n";
        }

         struct Major{
            string major;
            int freq;
        };


};

#endif