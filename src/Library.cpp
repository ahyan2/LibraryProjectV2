#include "../include/Library.h"
#include "../include/Book.h"
#include "../include/LibraryItem.h"
#include "../include/FileIO.h"
#include "../include/MyUtils.h"
#include "../include/ReferenceBook.h"
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
#include <stdexcept>

using namespace std;

Library::Library() : catalog()    // start with an empty map
{
    catalog.emplace(10001, make_unique<Book>(10001, "George Orwell", "Animal Farm", "Satire"));
    catalog.emplace(10002, make_unique<Book>(10002, "Harper Lee", "To Kill a Mockingbird", "Historic Fiction"));
    catalog.emplace(10003, make_unique<Book>(10003, "Suzanne Collins", "The Hunger Games", "Dystopia"));
    catalog.emplace(10004, make_unique<Book>(10004, "George Orwell", "1984", "Dystopia"));
    catalog.emplace(10005, make_unique<Book>(10005, "Andy Weir", "The Martian", "SciFi"));

    catalog.emplace(20001, make_unique<ReferenceBook>(20001, "Dr. Jane Foster", "Encyclopedia of Science", "Science"));
    catalog.emplace(20002, make_unique<ReferenceBook>(20002, "Oxford University Press", "Oxford English Dictionary", "Language"));
}

void Library::displayWelcome(){
    cout << setw(40) << setfill('=') << " " << setfill(' ') << endl;
    cout << "Hello, welcome to the virtual library!" << endl;
    this_thread::sleep_for(chrono::seconds(2));

    while (true){
        cout << "What is your first name?" << endl;
        name = MyUtils::validFirstName();

        cout << "What is your 8-digit student ID?" << endl;
        this_thread::sleep_for(chrono::seconds(1));
        cout << "*** NOTE: A valid student ID does not start with zero (0) ***" << endl;
        studentID = MyUtils::validID(8);

        cout << setw(40) << setfill('=') << " " << setfill(' ') << endl;
        cout << "Please confirm if this is your first name and SID: " << name << ", " << studentID << endl;
        this_thread::sleep_for(chrono::seconds(1));
        char choice = MyUtils::validChoice();
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

    for (LibraryItem* item : getCatalogSortedById()) {
        cout << item->summary() << endl;
    }
}

void Library::displayOverview() {
    cout << "Enter the 5‑digit book ID to view its overview: ";
    int bookID = MyUtils::validID(5);

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
        int bookID = MyUtils::validID(5);

        auto it = catalog.find(bookID);
        if (it == catalog.end()) {
            cout << "Invalid ID or not available. Try again.\n";
            continue;
        }

        LibraryItem &item = *it->second;
        cout << "*** " << item.summary() << " ***\n";
        cout << "Is this the book you would like to checkout?\n"
             << "*** " << item.summary() << " ***\n";
        char choice = MyUtils::validChoice();
        if (choice == 'y') {
            // move into smart pointer map
            checkedOutBooks[studentID] = std::move(it->second);

            // record into borrowHistory as a shared_ptr
            borrowHistory[studentID] = shared_ptr<LibraryItem>(checkedOutBooks[studentID].get(), [](LibraryItem*){}  // no double delete
    );
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
    unique_ptr<LibraryItem> returned = move(it->second);
    int id = returned->getId();
    catalog.emplace(id, std::move(returned));
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
        char choice = MyUtils::validChoice();
        if (choice == 'y'){
            checkIn();
            this_thread::sleep_for(chrono::seconds(2));
            cout << endl << "Thank you " << name << " for returning your book!" << endl << endl;
            break;
        }
    }
}

vector<LibraryItem*> Library::getCatalogSortedByTitle() const {
    vector<LibraryItem*> items;
    items.reserve(catalog.size());
    // collect every polymorphic item
    for (auto &p : catalog) {
        items.push_back(p.second.get());
    }
    // sort by summary (which for Book or ReferenceBook begins with the title or ID)
    sort(items.begin(), items.end(),
         [](LibraryItem* a, LibraryItem* b) {
             return a->summary() < b->summary();
         });
    return items;
}



vector<LibraryItem*> Library::getCatalogSortedByAuthor() const {
    vector<LibraryItem*> items;
    items.reserve(catalog.size());
    for (auto &p : catalog) {
        items.push_back(p.second.get());
    }
    // sort by author—downcast to Book or ReferenceBook to fetch author field
    sort(items.begin(), items.end(),
         [](LibraryItem* a, LibraryItem* b) {
             // both Book and ReferenceBook have getAuthor() or equivalent
             // first try Book, then ReferenceBook
             if (auto ba = dynamic_cast<Book*>(a)) {
                 if (auto bb = dynamic_cast<Book*>(b))
                     return ba->getAuthor() < bb->getAuthor();
             }
             if (auto ra = dynamic_cast<ReferenceBook*>(a)) {
                 if (auto rb = dynamic_cast<ReferenceBook*>(b))
                     return ra->getGenre() < rb->getGenre();
             }
             // fallback: summary comparison
             return a->summary() < b->summary();
         });
    return items;
}



void Library::showBorrowHistory() {
    // If this user has borrowed before, print the last one
    auto it = borrowHistory.find(studentID);
    if (it == borrowHistory.end()) {
        cout << "You have no borrow history.\n";
    } else {
        LibraryItem &item = *it->second;
        cout << "Your last borrowed book was: " << item.summary() << "\n";
    }
}

void Library::userCatalogInteraction() {
    while (true) {
        UserAction action = MyUtils::getUserAction();
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
                for (LibraryItem* item : getCatalogSortedById()) {  // sorting by ID before leaving program
                    cout << item->summary() << "\n";
                }
                displayGoodbye();
                return;

            case SortByTitle: {
                auto list = getCatalogSortedByTitle();
                cout << "\nCatalog sorted by title:\n";
                for (LibraryItem* item : list) {
                    cout << item->summary() << "\n";
                }

                break;
            }

            case SortByAuthor: {
                auto list = getCatalogSortedByAuthor();
                cout << "\nCatalog sorted by author:\n";
                for (LibraryItem* item : list) {
                    cout << item->summary() << "\n";
                }
                break;
            }
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void Library::loadCatalogBinary(const string& filename) {
    try {
        FileIO::loadCatalogBinary(filename, catalog);
    } catch (const exception& e) {
        cerr << "Warning: could not load catalog: " << e.what() << "\n";
    }
}

void Library::saveCatalogBinary(const string& filename) {
    try {
        FileIO::saveCatalogBinary(filename, catalog);
    } catch (const exception& e) {
        cerr << "Error: failed to save catalog: " << e.what() << "\n";
    }
}

void Library::saveFeedback(const string& feedback) {
    try {
        FileIO::saveFeedback(feedback);
    } catch (const exception& e) {
        cerr << "Error: could not save feedback: " << e.what() << "\n";
    }
}

void Library::exportCatalog(const string& filename, bool onlyAvailable) {
    try {
        FileIO::exportCatalog(filename, catalog, checkedOutBooks, onlyAvailable);
    } catch (const exception& e) {
        cerr << "Error: could not export catalog: " << e.what() << "\n";
    }
}


vector<LibraryItem*> Library::getCatalogSortedById() const {
    vector<LibraryItem*> items;
    items.reserve(catalog.size());
    for (auto &p : catalog) {
        items.push_back(p.second.get());
    }
    sort(items.begin(), items.end(),
         [](LibraryItem* a, LibraryItem* b) {
             return a->getId() < b->getId();
         });
    return items;
}
