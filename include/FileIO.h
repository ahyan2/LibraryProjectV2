#ifndef FILE_IO_H
#define FILE_IO_H

#include "Library.h"  // for Book, catalog, checkedOutBooks
#include "Book.h"
#include <unordered_map>
#include <stdexcept>

using namespace std;


class FileIO {
public:
    static void saveFeedback(const string &feedback);  // Append user feedback to a text file

    static void exportCatalog(const string &filename,
                        const unordered_map<int, Book>& catalog,
                        const unordered_map<int, unique_ptr<Book>>& checkedOut,
                        bool onlyAvailable); // Export catalog to CSV/TXT (all or only available)

    static void saveCatalogBinary(const string &filename, const unordered_map<int, Book>& catalog);     // Save current catalog to a binary file

    static void loadCatalogBinary(const string &filename, unordered_map<int, Book>& catalog);

};

#endif //FILE_IO_H
