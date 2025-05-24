#include "../include/ReferenceBook.h"
#include "LibraryItem.h"
#include <string>

using namespace std;

ReferenceBook::ReferenceBook(int id, string a, string t, string genre)
  : id(id), author(a), title(t), genre(genre) {}

int ReferenceBook::getId() const {
    return id;
}

string ReferenceBook::getGenre() const noexcept {
    return genre;
}

string ReferenceBook::summary() const {
    return "[" + to_string(id) + "] \"" + title +
           "\" by " + author + " (Ref on " + genre + ")";
}
