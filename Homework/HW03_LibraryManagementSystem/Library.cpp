/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Sat March 21 2026
 *  Desc: Library class defintions and implementations 
 *  File : Library.cpp
 *******************************************/

#ifndef LIBRARY_CPP
#define LIBRARY_CPP

#include <iostream>
#include <ranges>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include "Book.cpp"

using namespace std;

class Library {
    public:
        vector<Book> books;
        
        Library(){
        }

        Library(vector<Book> books) : books(books){
        }

        void add_book(const Book& book){
            books.push_back(book);
        }

        void remove_book(const string& title){
            books.erase( find_if(books.begin(), books.end(), [title](Book& book){
                return book.getTitle() == title;
            }) );
        }

        Book* find_book(const string& title){
            vector<Book>::iterator it = find_if(books.begin(), books.end(), [title](Book& book){ return book.getTitle()== title;});
            
            if(it == books.end()){
                cout << "\n\t \"" << title << "\" not found!" << endl;
                return nullptr;
            }

            return &(*it);
        }

        list<Book> get_books_by_author(const string& author){
            list<Book> books_by_author;

            for(vector<Book>::iterator it = books.begin(); it != books.end(); ++it){
                if(it->getAuthor() == author)
                    books_by_author.push_back(*it);
            }

            return books_by_author;
        }

        list<Book> get_books_by_genre(const string& genre){
            list<Book> books_by_genre;

            for(vector<Book>::iterator it = books.begin(); it != books.end(); ++it){
                if(it->getGenre() == genre)
                    books_by_genre.push_back(*it);
            }

            return books_by_genre;
        }

        vector<Book> filter_by_year(int year){
            vector<Book> filtered_books;

            auto filtered_view = books | std::views::filter([year](Book& book){
                return book.getYear() == year;
            });

            for(auto it = filtered_view.begin(); it != filtered_view.end(); ++it)
                filtered_books.push_back(*it);
            

            return filtered_books;
        }

        vector<Book> filter_by_genre(const string& genre){
            vector<Book> filtered_books;

            auto filtered_view = books | std::views::filter([genre](Book book){
                return book.getGenre() == genre;
            });


            for(auto it = filtered_view.begin(); it != filtered_view.end(); ++it)
                filtered_books.push_back(*it);

            return filtered_books;
        }
};

#endif