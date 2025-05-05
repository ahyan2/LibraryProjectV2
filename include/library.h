#ifndef LIBRARY_H
#define LIBRARY_H

#include "my_utils.h"
#include <string>   // for strings
#include <thread>   // for time delay
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <algorithm>    // for sort()
#include <map>

struct Book {
    int id;
    string author;
    string title;
    string genre;
};

extern unordered_map<int, Book> catalog;    //  bookID , Book

extern unordered_map<int, unique_ptr<Book>> checkedOutBooks;    // studentID, unique_ptr<Book>

extern map<int, shared_ptr<Book>> borrowHistory;   // borrow history by studentID, keeps a shared_ptr to each Book

void displayWelcome();

void displayGoodbye();

void displayOverview();

void displayBooks();

vector<Book> getCatalogSortedByTitle();

vector<Book> getCatalogSortedByAuthor();

void showBorrowHistory();   // each user’s last borrowed book

void userCatalogInteraction();




#endif
