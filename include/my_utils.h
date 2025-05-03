#ifndef MY_UTILS_H
#define MY_UTILS_H

#include <string>   // for strings
#include <thread>   // for time delay

using namespace std;

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
