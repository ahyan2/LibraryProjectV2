#include "../include/my_utils.h"

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


using namespace std;


string name;
int studentID;

unordered_map<int, Book> catalog = {
    {11111, {11111, "George Orwell",   "Animal Farm",           "Satire"}},
    {11112, {11112, "Harper Lee",      "To Kill a Mockingbird", "Historic Fiction"}},
    {11113, {11113, "Suzanne Collins", "The Hunger Games",      "Dystopia"}},
    {11114, {11114, "George Orwell",   "1984",                  "Dystopia"}},
    {11115, {11115, "Andy Weir",       "The Martian",           "SciFi"}}
};

unordered_map<int, unique_ptr<Book>> checkedOutBooks;


int validID(int givenDigits){
    int num;
    while (true){
        cin >> num;

        if (cin.fail() || num <= 0){
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid... Please enter the correct ID" << endl;
        } else {    // input is int and pos, now check if it is correct digits or not
            int digits = 0;
            int numCopy = num;
            while(numCopy > 0){
                numCopy /= 10;
                digits++;
            }
            if (digits == givenDigits){
                break;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid... Please enter the correct ID" << endl;
        }
    }
    return num;
}


string validFirstName(){
    string name;
    while (true) {
        getline(cin, name);
        bool isValid = true;

        if (!name.empty()) {    // if input is not empty, continue  with check
            for (char c : name) {   // check if each character is in the alphabet (only letters)
                if (!isalpha(static_cast<unsigned char>(c))) { // correct handling of char values
                    isValid = false;    // if any char is not a letter, set bool false, invalid input
                    break;
                }
            }
        }

        if (!isValid  || name.empty()) {    // invalid if not all letters or empty input
            cout << "Invalid... Please enter only your first name" << endl;
        } else {
            break;
        }
    }
    return name;
}


char validChoice(){
    char choice;
    while (true){
        cout << "Type 'y' for Yes, or 'n' for No." << endl;
        cin >> choice;

        choice = tolower(choice);
        if(choice == 'y' || choice == 'n'){
            cin.ignore(numeric_limits<streamsize>::max(), '\n');    // remove input buffer if user types multiple characters
            break;
        } else {
            cout << "Invalid... Please type either y or n " << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');        // remove input buffer if user types multiple characters
        }
    }
    return choice;
}


UserAction getUserAction() {
    int choice;
    while (true) {
        cout
          << "\nWhat would you like to do?\n"
          << "  1. Check out book\n"
          << "  2. Check in book\n"
          << "  3. View library catalog\n"
          << "  4. View overview of a book\n"
          << "  5. Leave\n"
          << "  6. Sort catalog by title\n"
          << "  7. Sort catalog by author\n"
          << "Enter choice [1-7]: ";


        if (!(cin >> choice) || choice < 1 || choice > 7) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter 1–5.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return static_cast<UserAction>(choice);
        }
    }
}



void displayWelcome(){
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



void displayGoodbye(){
    cout << setw(60) << setfill('*') << " " << setfill(' ') << endl;

    cout << "Thank you " << name << " for visiting our virtual library, We hope to see you again soon!" << endl;

    cout << setw(60) << setfill('*') << " " << setfill(' ') << endl;
}

void displayBooks() {
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
        cout << setw(10) << b.id
             << setw(25) << b.author
             << setw(20) << b.genre
             << b.title << endl;
    }
}



void checkOut() {
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
             << "*** " << b.title << ", by " << b.author << " ***\n";
        char choice = validChoice();
        if (choice == 'y') {
            // move into smart pointer map
            checkedOutBooks[studentID] = make_unique<Book>(b);
            catalog.erase(it);
            break;
        }
    }
}




bool checkIn() {
    auto it = checkedOutBooks.find(studentID);
    if (it == checkedOutBooks.end()) {
        cout << "No books to check in.\n";
        return false;
    }

    // move book back into catalog
    unique_ptr<Book> returned = move(it->second);
    catalog[returned->id] = *returned;
    checkedOutBooks.erase(it);

    displayBooks();
    return true;
}



void displayOverview() {
    cout << "Enter the 5‑digit book ID to view its overview: ";
    int bookID = validID(5);

    // Check if the book is in catalog or currently checked out by this user
    bool inCatalog    = catalog.count(bookID);
    bool inCheckedOut = checkedOutBooks.count(studentID) &&
                        checkedOutBooks[studentID]->id == bookID;
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


void awaitingCheckIn(){
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


void userCatalogInteraction() {
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
                    cout << b.id << " | " << b.author << " | "
                         << b.genre << " | " << b.title << "\n";
                }
                break;
            }

            case SortByAuthor: {
                auto list = getCatalogSortedByAuthor();
                cout << "\nCatalog sorted by author:\n";
                for (auto &b : list) {
                    cout << b.id << " | " << b.author << " | "
                         << b.genre << " | " << b.title << "\n";
                }
                break;
            }
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}

vector<Book> getCatalogSortedByTitle() {
    vector<Book> books;
    books.reserve(catalog.size());
    for (auto &p : catalog) {
        books.push_back(p.second);
    }
    sort(books.begin(), books.end(),
         [](const Book &a, const Book &b) {
             return a.title < b.title;
         });
    return books;
}

vector<Book> getCatalogSortedByAuthor() {
    vector<Book> books;
    books.reserve(catalog.size());
    for (auto &p : catalog) {
        books.push_back(p.second);
    }
    sort(books.begin(), books.end(),
         [](const Book &a, const Book &b) {
             return a.author < b.author;
         });
    return books;
}


// Append user feedback to a text file
void saveFeedback(const string &feedback) {
    // Open feedback.txt in append mode so we don’t overwrite existing feedback
    ofstream ofs("feedback.txt", ios::app);
    if (!ofs) {
        cout << "Error: Could not open feedback.txt for writing.\n";
        return;  // Early return on file open failure
    }
    ofs << feedback << "\n";  // Write the user's feedback, followed by a newline
}

// Export catalog to CSV/TXT
void exportCatalog(const string &filename, bool onlyAvailable) {
    // Open the given filename for text output (overwrites existing file)
    ofstream ofs(filename);
    if (!ofs) {
        cout << "Error: Could not open " << filename << " for writing.\n";
        return;  // Abort if file can't be created
    }

    // Write a header row for CSV format
    ofs << "ID,Author,Genre,Title,Available\n";

    // First, list all books still in the catalog (available)
    for (auto &p : catalog) {
        const Book &b = p.second;
        ofs << b.id << ','
            << b.author << ','
            << b.genre << ','
            << b.title << ",Yes\n";
    }

    // Then, if requested, list checked‑out books as unavailable
    if (!onlyAvailable) {
        for (auto &p : checkedOutBooks) {
            const Book &b = *p.second;
            ofs << b.id << ','
                << b.author << ','
                << b.genre << ','
                << b.title << ",No\n";
        }
    }
}

// Save current catalog to a binary file for efficient storage
void saveCatalogBinary(const string &filename) {
    // Open file in binary mode
    ofstream ofs(filename, ios::binary);
    if (!ofs) {
        cout << "Error: Could not open " << filename << " for binary write.\n";
        return;
    }

    // First write the number of Book entries
    size_t count = catalog.size();
    ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // Then write each Book’s data
    for (auto &p : catalog) {
        const Book &b = p.second;

        // Write the integer ID
        ofs.write(reinterpret_cast<const char*>(&b.id), sizeof(b.id));

        // Helper to write a string: first its length, then its characters
        auto writeString = [&](const string &s){
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        };

        // Write author, title, genre
        writeString(b.author);
        writeString(b.title);
        writeString(b.genre);
    }
}

// Load catalog from a binary file on startup
void loadCatalogBinary(const string &filename) {
    // Open file for binary read
    ifstream ifs(filename, ios::binary);
    if (!ifs) {
        // File doesn't exist yet—start with the default in-memory catalog
        return;
    }

    // Read how many Book entries were saved
    size_t count;
    ifs.read(reinterpret_cast<char*>(&count), sizeof(count));

    // Clear any existing in-memory catalog
    catalog.clear();

    // Read each saved Book back into the map
    for (size_t i = 0; i < count; ++i) {
        Book b;

        // Read the integer ID
        ifs.read(reinterpret_cast<char*>(&b.id), sizeof(b.id));

        // Helper to read a string: read length then characters
        auto readString = [&](string &s){
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            s.resize(len);
            ifs.read(&s[0], len);
        };

        // Read author, title, genre
        readString(b.author);
        readString(b.title);
        readString(b.genre);

        // Reinsert into the in-memory catalog
        catalog[b.id] = b;
    }
}

