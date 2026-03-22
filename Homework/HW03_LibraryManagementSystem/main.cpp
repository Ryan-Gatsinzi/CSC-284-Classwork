/******************************************
 *  Author : Ryan Gatsinzi Udatinya
 *  Last updated on : Sat March 21 2026
 *  Desc: testing library class, printing and filtering 
 *  File : main.cpp
 *******************************************/

#include "Library.cpp"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <vector>

using namespace std;

void prompt();
int getPosNum();
int getChoice(int min, int max);
void filterByGenre(Library &library);
void filterByYear(Library &library);
list<string> sortByYear(const list<Book> &books);
char getYorN();

template <typename T> void print(const T &container);
void printList(list<string> &sortedList);

int main() {
  Library library{vector<Book>{
      Book("The Silent Archipelago", "Elena Vance", "Science Fiction", 2021),
      Book("Nebula Drift", "Kaelen Voss", "Science Fiction", 2015),
      Book("Alpha Protocol", "Sora Tanaka", "Science Fiction", 2021),
      Book("Echoes of the Loom", "Julian Thorne", "Historical Fiction", 2018),
      Book("The Glass Bridge", "Sarah Jenkins", "Mystery", 2018),
      Book("Vintage Skies", "Clara Montrose", "Romance", 2018),
      Book("Shadows of Aetheria", "Lyra Belacqua", "Fantasy", 2015),
      Book("The Iron Crown", "Elias Thorne", "Fantasy", 2023),
      Book("The Last Hearth", "Sarah J. Maas", "Post-Apocalyptic", 2021),
      Book("Digital Silence", "Marcus Sterling", "Cyberpunk", 2021),
      Book("Solar Reign", "Ava Sinclair", "Science Fiction", 2001),
      Book("Whispers of Eldoria", "Lucien Vire", "Fantasy", 2003),
      Book("Cracked Reflections", "Maya Chen", "Mystery", 1998),
      Book("Ashes of Tomorrow", "Rafael Costa", "Post-Apocalyptic", 2002),
      Book("Neon Pulse", "Iris Kwon", "Cyberpunk", 2011),
      Book("The Sapphire Pact", "Elio Romano", "Fantasy", 2004),
      Book("Fading Letters", "Clara Weiss", "Romance", 2000),
      Book("Ironbound Secrets", "Darius Cole", "Historical Fiction", 2005),
      Book("Orbital Silence", "Nadia Petrov", "Science Fiction", 2002),
      Book("Velvet Horizon", "Jonah Blake", "Romance", 2022)}};

  int choice;
  char goOn = 'y';

  while (goOn == 'y' || goOn == 'Y') {
    prompt();
    cout << "\nEnter: ";
    choice = getChoice(1, 3);

    switch (choice) {
    case 1:
      print(library.books);
      break;
    case 2:
      filterByGenre(library);
      break;
    case 3:
      filterByYear(library);
      break;
    }

    cout << "\nDo you want to continue? ";
    goOn = getYorN();
  }
}

void prompt() {
  cout << "\nWelcome to my library! what would you like to do? \n"
       << "1. View all books \n"
       << "2. Filter by genre\n"
       << "3. Filter by year \n";
}

int getPosNum() {
  int num;
  cin >> num;
  cin.ignore(10000, '\n');

  while (num < 0) {
    cout << "\t\t Enter a positve number please: ";
    cin >> num;
    cin.ignore(10000, '\n');
  }

  return num;
}

char getYorN() {
  char y_or_n = cin.get();
  cin.ignore(1000, '\n');

  while (!(y_or_n == 'y' || y_or_n == 'Y' || y_or_n == 'n' || y_or_n == 'n')) {
    cout << "\t Invalid input. Enter y or n: ";
    y_or_n = cin.get();
    cin.ignore(1000, '\n');
  }

  return y_or_n;
}

int getChoice(int min, int max) {
  int choice = getPosNum();

  while (choice < min || choice > max) {
    cout << " \t\t Please enter a number between " << min << " and " << max
         << " : ";
    choice = getPosNum();
  }

  return choice;
}

template <typename T> void print(const T &container) {
  for (auto it = container.begin(); it != container.end(); ++it)
    cout << it->toString() << endl;
}

void printList(list<string> &sortedList) {
  for (list<string>::iterator it = sortedList.begin(); it != sortedList.end();
       ++it) {
    cout << *it << endl;
  }
}

list<string> sortByYear(vector<Book> books) { // intentional copy
  list<string> sortedBooks;

  std::ranges::sort(books, {}, &Book::getYear);

  std::ranges::transform(books, back_inserter(sortedBooks),
                         [](Book &book) { return book.toString(); });

  return sortedBooks;
}

void filterByGenre(Library &library) {
  string genre;
  char y_or_n;
  vector<Book> filtered_books;

  cout << "What genre do you want to filter by: ";
  getline(cin, genre);

  filtered_books = library.filter_by_genre(genre);
  print(filtered_books);

  cout << "\n Do you want the books sorted by year (y/n): ";
  y_or_n = getYorN();

  if (y_or_n == 'y' || y_or_n == 'Y') {
    list<string> stringOfSortedBooks = sortByYear(filtered_books);
    printList(stringOfSortedBooks);
  }
}

void filterByYear(Library &library) {
  int year;

  cout << "What year do you want to filter by: ";
  cin >> year;
  cin.ignore(1000, '\n');

  print(library.filter_by_year(year));
}
