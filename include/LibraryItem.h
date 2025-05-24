#ifndef LIBRARYITEM_H
#define LIBRARYITEM_H

#include <string>

using namespace std;

class LibraryItem {
public:
    virtual ~LibraryItem() = default;

    // return item’s unique ID
    virtual int getId() const = 0;

    // return one line summary of book ID, title, author
    virtual string summary() const = 0;
};

#endif
