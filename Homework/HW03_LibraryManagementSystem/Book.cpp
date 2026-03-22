/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Sat March 21 2026
 *  Desc: Book class defintions and implementations 
 *  File : Book.cpp
 *******************************************/

#ifndef BOOK_CPP
#define BOOK_CPP

#include <string>
using namespace std;

class Book {
    private:
        string title, author, genre;
        int year;

    public:
        Book(string title, string author, string genre, int year) : title(title), author(author), genre(genre), year(year){}

        void setTitle(string title) {
            this->title = title;
        }

        void setAuthor(string author) {
            this->author = author;
        }

        void setGenre(string genre) {
            this->genre = genre;
        }

        void setYear(int year) {
            this->year = year;
        }

        string getTitle(){ return title; }

        string getAuthor(){ return author; }

        string getGenre(){ return genre; }

        int getYear(){ return year; }

        string toString() const{ 
            return title + " by " + author + " (" + to_string(year) + ") [" + genre + "]";
        }
};

#endif