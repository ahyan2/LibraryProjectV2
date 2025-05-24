#ifndef LIBRARY_H
#define LIBRARY_H

#include "MyUtils.h"
#include "Book.h"
#include "FileIO.h"
#include "Library.h"
#include "LibraryItem.h"
#include <string>   // for strings
#include <thread>   // for time delay
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <algorithm>    // for sort()
#include <map>
#include <stdexcept>

using namespace std;

class Library {
private:
    string name;
    int studentID;

    unordered_map<int, unique_ptr<LibraryItem>> catalog;    //  bookID , Book

    unordered_map<int, unique_ptr<LibraryItem>> checkedOutBooks;    // studentID, unique_ptr<Book>

    map<int, shared_ptr<LibraryItem>> borrowHistory;   // borrow history by studentID, keeps a shared_ptr to each Book

public:
    Library();  // default constructor

    void displayWelcome();

    void displayGoodbye();

    void displayOverview();

    void displayBooks();

    vector<LibraryItem*> getCatalogSortedByTitle() const;

    vector<LibraryItem*> getCatalogSortedByAuthor() const;

    void showBorrowHistory();   // each user’s last borrowed book

    void userCatalogInteraction();

    void checkOut();

    bool checkIn();

    void awaitingCheckIn();

    // IO wrapper functions
    void loadCatalogBinary(const string& filename);
    void saveCatalogBinary(const string& filename);
    void saveFeedback(const string& feedback);
    void exportCatalog(const string& filename, bool onlyAvailable);

};

#endif
