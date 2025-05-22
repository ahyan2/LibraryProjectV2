#include "../include/Library.h"
#include "../include/Book.h"
#include <iostream> // for print
#include <string>   // for strings
#include <iomanip>  // for display manip
#include <chrono>   // for time delay
#include <thread>   // for time delay
#include <limits>
#include <unordered_map>
#include <memory>
#include <vector>
#include <algorithm>
#include <fstream>
#include <map>

using namespace std;

Library::Library() : catalog{
        {11111, Book{11111, "George Orwell",   "Animal Farm",           "Satire"}},
        {11112, Book{11112, "Harper Lee",      "To Kill a Mockingbird", "Historic Fiction"}},
        {11113, Book{11113, "Suzanne Collins", "The Hunger Games",      "Dystopia"}},
        {11114, Book{11114, "George Orwell",   "1984",                  "Dystopia"}},
        {11115, Book{11115, "Andy Weir",       "The Martian",           "SciFi"}}
    }
{}

void Library::displayWelcome(){
    cout << setw(40) << setfill('=') << " " << setfill(' ') << endl;
    cout << "Hello, welcome to the virtual library!" << endl;
    this_thread::sleep_for(chrono::seconds(2));

    while (true){
        cout << "What is your first name?" << endl;
        name = validFirstName();

        cout << "What is your 8-digit student ID?" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        cout << "*** NOTE: A valid student ID does not start with zero (0) ***" << endl;
        studentID = validID(8);

        cout << setw(40) << setfill('=') << " " << setfill(' ') << endl;
        cout << "Please confirm if this is your first name and SID: " << name << ", " << studentID << endl;
        this_thread::sleep_for(chrono::seconds(1));
        char choice = validChoice();
        if (choice == 'y'){
            break;
        } else {
            cout << "Got it, let's start over." << endl;
            cout << setw(40) << setfill('=') << " " << setfill(' ') << endl;
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

}



void Library::displayGoodbye(){
    cout << setw(60) << setfill('*') << " " << setfill(' ') << endl;

    cout << "Thank you " << name << " for visiting our virtual library, We hope to see you again soon!" << endl;

    cout << setw(60) << setfill('*') << " " << setfill(' ') << endl;
}

void Library::displayBooks() {
    cout << setw(40) << setfill('=') << " " << setfill(' ') << endl;
    this_thread::sleep_for(chrono::seconds(1));
    cout << "Hello " << name << ", here is our library catalog!" << endl << endl;

    cout << right << setw(47) << "* Current Books *" << endl;
    cout << left  << setw(10) << "ID"
                  << setw(25) << "Author"
                  << setw(20) << "Genre"
                  << "Title" << endl;
    cout << setw(80) << setfill('-') << "" << setfill(' ') << endl;

    for (const auto &entry : catalog) {
        const Book &b = entry.second;
        cout << setw(10) << b.getId()
             << setw(25) << b.getAuthor()
             << setw(20) << b.getGenre()
             << b.getTitle() << endl;
    }
}

void Library::displayOverview() {
    cout << "Enter the 5‑digit book ID to view its overview: ";
    int bookID = validID(5);

    // Check if the book is in catalog or currently checked out by this user
    bool inCatalog    = catalog.count(bookID);
    bool inCheckedOut = checkedOutBooks.count(studentID) &&
                        checkedOutBooks[studentID]->getId() == bookID;
    if (!inCatalog && !inCheckedOut) {
        cout << "Book ID not found in catalog or your checked‑out list.\n";
        return;
    }

    // Build the path: overviews/<ID>.txt
    string filename = "../overviews/" + to_string(bookID) + ".txt"; // go up from working directory (cmake debug) then proceed
    ifstream ifs(filename);
    if (!ifs.is_open()) {
        cout << "No overview file found for book " << bookID << ".\n";
        return;
    }

    cout << "\n─── Overview for Book " << bookID << " ───\n";
    string line;
    while (getline(ifs, line)) {
        cout << line << "\n";
    }
    cout << "────────────────────────────────────\n";
}

void Library::checkOut() {
    // ensure one book per user
    if (checkedOutBooks.count(studentID)) {
        cout << "You already have a book checked out. Return it before checking out another.\n";
        return;
    }

    while (true) {
        cout << "Enter the 5‑digit book ID:" << endl;
        int bookID = validID(5);

        auto it = catalog.find(bookID);
        if (it == catalog.end()) {
            cout << "Invalid ID or not available. Try again.\n";
            continue;
        }

        const Book &b = it->second;
        cout << "Is this the book you would like to checkout?\n"
             << "*** " << b.getTitle() << ", by " << b.getTitle() << " ***\n";
        char choice = validChoice();
        if (choice == 'y') {
            // move into smart pointer map
            checkedOutBooks[studentID] = make_unique<Book>(b);

            // record into borrowHistory as a shared_ptr
            borrowHistory[studentID] = make_shared<Book>(b);
            showBorrowHistory();

            catalog.erase(it);
            break;
        }
    }
}




bool Library::checkIn() {
    auto it = checkedOutBooks.find(studentID);
    if (it == checkedOutBooks.end()) {
        cout << "No books to check in.\n";
        return false;
    }

    // move book back into catalog
    unique_ptr<Book> returned = move(it->second);
    catalog[returned->getId()] = *returned;
    checkedOutBooks.erase(it);

    displayBooks();
    return true;
}



void Library::awaitingCheckIn(){
    this_thread::sleep_for(chrono::seconds(1));
    for(int i = 0; i < 15; i++){
        cout << '.' << endl;
    }
    int daysPassed = 1;
    while (daysPassed <= 7){
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Days passed since checkout: "<< daysPassed << endl;
        daysPassed++;
    }
    while(true){
        this_thread::sleep_for(chrono::seconds(1));
        cout << "Hello " << name << " (SID " << studentID << "), your book is overdue! Please check it back in." << endl;
        cout << endl << "Would you like to check-in your book?" << endl;
        char choice = validChoice();
        if (choice == 'y'){
            checkIn();
            this_thread::sleep_for(chrono::seconds(2));
            cout << endl << "Thank you " << name << " for returning your book!" << endl << endl;
            break;
        }
    }
}

vector<Book> Library::getCatalogSortedByTitle() {
    vector<Book> books;
    books.reserve(catalog.size());
    for (auto &p : catalog) {
        books.push_back(p.second);
    }
    sort(books.begin(), books.end(),
         [](const Book &a, const Book &b) {
             return a.getTitle() < b.getTitle();
         });
    return books;
}

vector<Book> Library::getCatalogSortedByAuthor() {
    vector<Book> books;
    books.reserve(catalog.size());
    for (auto &p : catalog) {
        books.push_back(p.second);
    }
    sort(books.begin(), books.end(),
         [](const Book &a, const Book &b) {
             return a.getAuthor() < b.getAuthor();
         });
    return books;
}

void Library::showBorrowHistory() {
    // If this user has borrowed before, print the last one
    auto it = borrowHistory.find(studentID);
    if (it == borrowHistory.end()) {
        cout << "You have no borrow history.\n";
    } else {
        auto& b = *it->second;  // shared_ptr<Book>
        cout << "Your last borrowed book was: ["
             << b.getId() << "] " << b.getTitle() << " by " << b.getAuthor() << "\n";
    }
}

void Library::userCatalogInteraction() {
    while (true) {
        UserAction action = getUserAction();
        switch (action) {
            case CheckOut:
                checkOut();
                break;

            case CheckIn:
                if (!checkIn())
                    cout << "Nothing to check in.\n";
                break;

            case ViewLibrary:
                displayBooks();
                break;

            case ViewOverview:
                displayOverview();
                break;

            case LeaveProgram:
                if (checkedOutBooks.count(studentID)) {
                    cout << "You still have a book checked out. Please return it before exiting.\n";
                    break;
                }
                displayGoodbye();
                return;

            case SortByTitle: {
                auto list = getCatalogSortedByTitle();
                cout << "\nCatalog sorted by title:\n";
                for (auto &b : list) {
                    cout << b.getId() << " | " << b.getAuthor() << " | "
                         << b.getGenre() << " | " << b.getTitle() << "\n";
                }
                break;
            }

            case SortByAuthor: {
                auto list = getCatalogSortedByAuthor();
                cout << "\nCatalog sorted by author:\n";
                for (auto &b : list) {
                    cout << b.getId() << " | " << b.getAuthor() << " | "
                         << b.getGenre() << " | " << b.getTitle() << "\n";
                }
                break;
            }
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}