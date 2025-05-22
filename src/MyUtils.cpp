#include "../include/MyUtils.h"

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


int MyUtils::validID(int givenDigits){
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


string MyUtils::validFirstName(){
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


char MyUtils::validChoice(){
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


UserAction MyUtils::getUserAction() {
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


