#include "Book.h"
using namespace std;

Book::Book(int id,
           const string& author,
           const string& title,
           const string& genre)
  : id(id),
    author(author),
    title(title),
    genre(genre)
{}

int Book::getId() const {
    return id;
}

string Book::getAuthor() const {
    return author;
}

string Book::getTitle() const {
    return title;
}

string Book::getGenre() const {
    return genre;
}
