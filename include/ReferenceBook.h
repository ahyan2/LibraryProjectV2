#ifndef REFERENCEBOOK_H
#define REFERENCEBOOK_H
#include "LibraryItem.h"
#include <string>

using namespace std;

class ReferenceBook : public LibraryItem {
private:
    int id;
    string title;
    string author;
    string genre;

public:
    ReferenceBook(int id, string author, string title, string genre);
    int getId() const override;
    string summary()  const override;
    string getGenre() const noexcept;
    string getAuthor() const noexcept;
    string getTitle() const noexcept;
};


#endif
