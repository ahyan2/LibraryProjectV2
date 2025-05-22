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

class MyUtils {
public:
    // Reads and validates a choice [1–5], returns a UserAction
    static UserAction getUserAction();

    static int validID(int givenDigits);

    static string validFirstName();

    static char validChoice();

};

#endif
