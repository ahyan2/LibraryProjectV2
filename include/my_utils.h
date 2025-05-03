#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <string>   // for strings
#include <thread>   // for time delay

using namespace std;

enum UserAction {
    CheckOut = 1,  // Check out a book
    CheckIn = 2,  // Check in a book
    ViewLibrary = 3,  // View entire catalog
    ViewOverview = 4,  // View overview of a book
    LeaveProgram = 5   // Leave the program
};

// Reads and validates a choice [1–5], returns a UserAction
UserAction getUserAction();



extern string name;
extern int studentID;

extern vector<int> numberID;
extern vector<string> author;
extern vector<string> title;
extern vector<string> genre;


extern vector<int> removedNumberID;
extern vector<string> removedAuthor;
extern vector<string> removedTitle;
extern vector<string> removedGenre;


int validID(int givenDigits);

string validFirstName();

char validChoice();

void userCatalogInteraction();

void displayWelcome();

void displayGoodbye();

void displayBooks();

void checkOut();

bool checkIn();

void awaitingCheckIn();


#endif
