#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

class Book {
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
    int    getId()    const;
    string getAuthor() const;
    string getTitle()  const;
    string getGenre()  const;
};

#endif
