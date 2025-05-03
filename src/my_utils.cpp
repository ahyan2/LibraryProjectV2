#include "../include/my_utils.h"

#include <iostream> // for print
#include <string>   // for strings
#include <iomanip>  // for display manip
#include <chrono>   // for time delay
#include <thread>   // for time delay


using namespace std;


string name;
int studentID;

vector<Book> catalog = {
    {11111, "George Orwell",   "Animal Farm",            "Satire"},
    {11112, "Harper Lee",      "To Kill a Mockingbird",  "Historic Fiction"},
    {11113, "Suzanne Collins", "The Hunger Games",       "Dystopia"},
    {11114, "George Orwell",   "1984",                   "Dystopia"},
    {11115, "Andy Weir",       "The Martian",            "SciFi"}
};

vector<Book> removedBooks;


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
          << "Enter choice [1-5]: ";

        if (!(cin >> choice) || choice < 1 || choice > 5) {
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

void displayBooks(){
    cout << setw(40) << setfill('=') << " " << setfill(' ') << endl;

    this_thread::sleep_for(chrono::seconds(1));
    cout << "Hello " << name << ", here is our library catalog!" << endl << endl;

    cout << right << setw(47) << "* Current Books *" << endl;
    cout << left  << setw(10) << "ID"
                  << setw(25) << "Author"
                  << setw(20) << "Genre"
                  << "Title" << endl;
    cout << setw(80) << setfill('-') << "" << setfill(' ') << endl;

    for (const auto &book : catalog) {
        cout << setw(10) << book.id
             << setw(25) << book.author
             << setw(20) << book.genre
             << book.title << endl;
    }
}


void checkOut() {
    // Prevent double‑checkout
    if (!removedBooks.empty()) {
        cout << "You already have a book checked out. Return it before checking out another.\n";
        return;
    }

    while (true) {
        cout << "Enter the 5‑digit book ID:" << endl;
        int bookID = validID(5);

        // Manually search for the book by index
        int idx = -1;
        for (int i = 0; i < (int)catalog.size(); ++i) {
            if (catalog[i].id == bookID) {
                idx = i;
                break;
            }
        }
        if (idx < 0) {
            cout << "Invalid ID or not available. Try again." << endl;
            continue;
        }

        cout << "Is this the book you would like to checkout?" << endl;
        cout << "*** " << catalog[idx].title << ", by " << catalog[idx].author << " ***" << endl;
        char choice = validChoice();
        if (choice == 'y') {
            // move out of catalog into removedBooks
            removedBooks.push_back(catalog[idx]);
            catalog.erase(catalog.begin() + idx);
            break;
        }
    }
}





bool checkIn() {
    // Prevent check‑in when none checked out
    if (removedBooks.empty()) {
        cout << "No books to check in.\n";
        return false;
    }

    // Take the first removed book
    Book toReturn = removedBooks[0];
    removedBooks.erase(removedBooks.begin());

    // Manually find insertion point so catalog stays sorted by ID
    int insertPos = (int)catalog.size();  // default to end
    for (int i = 0; i < (int)catalog.size(); ++i) {
        if (toReturn.id < catalog[i].id) {
            insertPos = i;
            break;
        }
    }

    // Insert back into catalog at the correct position
    catalog.insert(catalog.begin() + insertPos, toReturn);

    displayBooks();
    return true;
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
                cout << "Overview feature coming soon.\n";
                break;

            case LeaveProgram:
                if (!removedBooks.empty()) {
                    cout << "You still have a book checked out. Please return it before exiting.\n";
                    break;
                }
                displayGoodbye();
                return;
        }
        this_thread::sleep_for(chrono::seconds(1));
    }
}


