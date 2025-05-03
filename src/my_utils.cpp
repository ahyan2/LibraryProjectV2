#include "../include/my_utils.h"

#include <iostream> // for print
#include <string>   // for strings
#include <iomanip>  // for display manip
#include <chrono>   // for time delay
#include <thread>   // for time delay


using namespace std;


string name;
int studentID;

vector<int> numberID = {1, 2, 3, 4, 5};;
vector<string> author = {"George Orwell", "Harper Lee", "Suzanne Collins", "George Orwell", "Andy Weir"};;
vector<string> title = {"Animal Farm", "To Kill a Mockingbird", "The Hunger Games", "1984", "The Martian"};
vector<string> genre = {"Satire", "Historic Fiction", "Dystopia", "Dystopia", "SciFi" };


vector<int> removedNumberID;
vector<string> removedAuthor;
vector<string> removedTitle;
vector<string> removedGenre;


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

    cout << left << setw(20) << "ID" << setw(20) << "Author" << setw(20) << "Genre" << "Title" << endl;

    cout << setw(80) << setfill('-') << "" << endl;

    for(int i = 0; i < numberID.size(); i++){
        cout << right << setw(5) << setfill('1') << numberID[i] << setfill(' ') << setw(10) << "" << left << setw(20) << author[i] << setw(20) << genre[i] << setw(20) << title[i] << endl;
    }

}


void checkOut(){
    while (true){
        cout << "Enter the 5-digit book ID:" << endl;
        int arrIndex = -1;
        while(true){
            int bookID = validID(5);
            if (bookID >= 11111 && bookID <= 11115){
                arrIndex = (bookID % 10) - 1;
                break;
            } else {
                cout << "Invalid, try again." << endl;
            }
        }
        cout << "Is this the book you would like to checkout?" << endl;
        cout << "*** " << title[arrIndex] << ", by " << author[arrIndex] << " ***" << endl;
        char choice = validChoice();
        if (choice == 'y'){
            removedNumberID.push_back(numberID[arrIndex]);      // adds value at arrIndex to vector
            removedAuthor.push_back(author[arrIndex]);
            removedGenre.push_back(genre[arrIndex]);
            removedTitle.push_back(title[arrIndex]);

            numberID.erase(numberID.begin() + arrIndex);     // removes value at arrIndex from vector
            author.erase(author.begin() + arrIndex);
            genre.erase(genre.begin() + arrIndex);
            title.erase(title.begin() + arrIndex);
            break;
        }
    }
}


bool checkIn(){
    for (int index = 0; index < numberID.size();  index++){
        if (removedNumberID[0] < numberID[index]){
            numberID.insert(numberID.begin() + index, removedNumberID[0]);
            author.insert(author.begin() + index, removedAuthor[0]);
            genre.insert(genre.begin() + index, removedGenre[0]);
            title.insert(title.begin() + index, removedTitle[0]);

            removedNumberID.erase(removedNumberID.begin());
            removedAuthor.erase(removedAuthor.begin());
            removedGenre.erase(removedGenre.begin());
            removedTitle.erase(removedTitle.begin());

            displayBooks();     // will only display books again once user checks book back in via this function.
            return true;    // doesn't execute final code segment if book info already inserted (true value doesn't matter)
        }
    }
    numberID.push_back(removedNumberID[0]);
    author.push_back(removedAuthor[0]);
    genre.push_back(removedGenre[0]);
    title.push_back(removedTitle[0]);

    displayBooks();
    return true;    // in case element removed was largest in original arrays. (true value doesn't matter)
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


void userCatalogInteraction(){
    this_thread::sleep_for(chrono::seconds(2));
    cout << endl << "Would you like to checkout a book?" << endl;
    char choice = validChoice();
    if (choice == 'y'){
        checkOut();
        this_thread::sleep_for(chrono::seconds(1));
        cout << endl;
        displayBooks();

        this_thread::sleep_for(chrono::seconds(1));

        cout << endl << "Would you like to check-in your book?" << endl;       // only able to check in book after checking out a book
        choice = validChoice();
        if (choice == 'y'){
            checkIn();
            this_thread::sleep_for(chrono::seconds(1));
            cout << endl << "Thank you " << name << " for returning your book!" << endl << endl;
            this_thread::sleep_for(chrono::seconds(1));
        } else{
            awaitingCheckIn();
        }
    }
}