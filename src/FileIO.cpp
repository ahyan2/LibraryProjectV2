#include "../include/FileIO.h"
#include "../include/Book.h"
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
#include <stdexcept>

using namespace std;


// Append user feedback to a text file
void FileIO::saveFeedback(const string &feedback) {
    // Open feedback.txt in append mode so we don’t overwrite existing feedback
    ofstream ofs("feedback.txt", ios::app);
    if (!ofs) {
        throw runtime_error("Could not open feedback.txt for writing");
    }
    ofs << feedback << "\n";  // Write the user's feedback, followed by a newline
}

// Export catalog to CSV/TXT
void FileIO::exportCatalog(const string &filename, const unordered_map<int, Book> &catalog,
                            const unordered_map<int, unique_ptr<Book>> &checkedOut, bool onlyAvailable) {
    // Open the given filename for text output (overwrites existing file)
    ofstream ofs(filename);
    if (!ofs) {
        throw runtime_error("Could not open " + filename + " for writing");
    }

    // Write a header row for CSV format
    ofs << "ID,Author,Genre,Title,Available\n";

    // First, list all books still in the catalog (available)
    for (auto &p : catalog) {
        const Book &b = p.second;
        ofs << b.getId() << ','
            << b.getAuthor() << ','
            << b.getGenre() << ','
            << b.getTitle() << ",Yes\n";
    }

    // Then, if requested, list checked‑out books as unavailable
    if (!onlyAvailable) {
        for (auto &p : checkedOut) {
            const Book &b = *p.second;
            ofs << b.getId() << ','
                << b.getAuthor() << ','
                << b.getGenre() << ','
                << b.getTitle() << ",No\n";
        }
    }
}

// Save current catalog to a binary file for efficient storage
void FileIO::saveCatalogBinary(const string &filename, const unordered_map<int, Book> &catalog) {
    // Open file in binary mode
    ofstream ofs(filename, ios::binary);
    if (!ofs) {
        throw runtime_error("Could not open " + filename + " for binary write");
    }

    // First write the number of Book entries
    size_t count = catalog.size();
    ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // Then write each Book’s data
    for (auto &p : catalog) {
        const Book &b = p.second;

        // Write the integer ID
        int id = b.getId();
        ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));

        // Helper to write a string: first its length, then its characters
        auto writeString = [&](const string &s){
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        };

        // Write author, title, genre
        writeString(b.getAuthor());
        writeString(b.getTitle());
        writeString(b.getGenre());
    }
}

// Load catalog from a binary file on startup
void FileIO::loadCatalogBinary(const string &filename, unordered_map<int, Book> &catalog) {
    // Open file for binary read
    ifstream ifs(filename, ios::binary);
    if (!ifs) {
        // File doesn't exist yet
        return;
    }

    // Read how many Book entries were saved
    size_t count;
    ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (ifs.fail()) {   // after reading count
        throw runtime_error("Failed to read catalog size from " + filename);
    }

    // Clear any existing in-memory catalog
    catalog.clear();

    // Read each saved Book back into the map
    for (size_t i = 0; i < count; ++i) {
        // 1) Read raw data into locals
        int id;
        ifs.read(reinterpret_cast<char*>(&id), sizeof(id));
        if (ifs.fail()) {   // after reading count
            throw runtime_error("Failed to read book ID from " + filename);
        }

        auto readString = [&](string &s){
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (ifs.fail()) {   // after reading count
                throw runtime_error("Failed to read string length from " + filename);
            }
            s.resize(len);
            ifs.read(&s[0], len);
            if (ifs.fail()) {   // after reading count
                throw runtime_error("Failed to read string data from " + filename);
            }
        };

        string author, title, genre;
        readString(author);
        readString(title);
        readString(genre);

        // 2) Construct a Book and insert
        Book b(id, author, title, genre);
        catalog[id] = b;
    }
}
