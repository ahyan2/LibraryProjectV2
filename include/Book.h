#ifndef BOOK_H
#define BOOK_H

#include <string>
#include "LibraryItem.h"
using namespace std;

class Book : public LibraryItem {
private:
    int    id;
    string author;
    string title;
    string genre;

public:
    // constructor
    Book() = default;
    Book(int id,
         const string& author,
         const string& title,
         const string& genre);

    // 2) getters
    string getAuthor() const;
    string getTitle()  const;
    string getGenre()  const;
    int getId() const override; // from LibraryItem override

    string summary() const override;
};

#endif
