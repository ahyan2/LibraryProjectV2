#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <string>   // for strings
#include <thread>   // for time delay
#include <vector>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <algorithm>    // for sort()
#include <map>


using namespace std;

enum UserAction {
    CheckOut = 1,  // Check out a book
    CheckIn = 2,  // Check in a book
    ViewLibrary = 3,  // View entire catalog
    ViewOverview = 4,  // View overview of a book
    LeaveProgram = 5,   // Leave the program
    SortByTitle  = 6,   // sort catalog by title
    SortByAuthor = 7    // sort catalog by author
};

// Reads and validates a choice [1–5], returns a UserAction
UserAction getUserAction();



extern string name;
extern int studentID;

struct Book {
    int id;
    string author;
    string title;
    string genre;
};


extern unordered_map<int, Book> catalog;    //  bookID , Book

extern unordered_map<int, unique_ptr<Book>> checkedOutBooks;    // studentID, unique_ptr<Book>

extern map<int, shared_ptr<Book>> borrowHistory;   // borrow history by studentID, keeps a shared_ptr to each Book

void showBorrowHistory();   // each user’s last borrowed book

void saveFeedback(const string &feedback);  // Append user feedback to a text file

void exportCatalog(const string &filename, bool onlyAvailable); // Export catalog to CSV/TXT (all or only available)

void saveCatalogBinary(const string &filename);     // Save current catalog to a binary file

void loadCatalogBinary(const string &filename);

vector<Book> getCatalogSortedByTitle();

vector<Book> getCatalogSortedByAuthor();

int validID(int givenDigits);

string validFirstName();

char validChoice();

void userCatalogInteraction();

void displayWelcome();

void displayGoodbye();

void displayOverview();

void displayBooks();

void checkOut();

bool checkIn();

void awaitingCheckIn();


#endif
