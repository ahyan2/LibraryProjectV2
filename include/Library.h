#ifndef LIBRARY_H
#define LIBRARY_H

#include "my_utils.h"
#include "Book.h"
#include <string>   // for strings
#include <thread>   // for time delay
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <algorithm>    // for sort()
#include <map>

class Library {
private:
    unordered_map<int, Book> catalog;    //  bookID , Book

    unordered_map<int, unique_ptr<Book>> checkedOutBooks;    // studentID, unique_ptr<Book>

    map<int, shared_ptr<Book>> borrowHistory;   // borrow history by studentID, keeps a shared_ptr to each Book

public:
    Library();  // default constructor

    void displayWelcome();

    void displayGoodbye();

    void displayOverview();

    void displayBooks();

    vector<Book> getCatalogSortedByTitle();

    vector<Book> getCatalogSortedByAuthor();

    void showBorrowHistory();   // each user’s last borrowed book

    void userCatalogInteraction();

    void checkOut();

    bool checkIn();

    void awaitingCheckIn();

};

#endif
