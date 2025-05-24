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
void FileIO::exportCatalog(const string &filename, const unordered_map<int, unique_ptr<LibraryItem>> &catalog,
                            const unordered_map<int, unique_ptr<LibraryItem>> &checkedOut, bool onlyAvailable) {
    // Open the given filename for text output (overwrites existing file)
    ofstream ofs(filename);
    if (!ofs) {
        throw runtime_error("Could not open " + filename + " for writing");
    }

    // Write a header row for CSV format
    ofs << "ID,Author,Genre,Title,Available\n";

    // First, list all books still in the catalog (available)
    for (auto &p : catalog) {
        // downcast to Book to access fields
        Book* b = dynamic_cast<Book*>(p.second.get());
        if (!b) continue; // skip non-Book items
        ofs << b->getId() << ','
            << b->getAuthor() << ','
            << b->getGenre() << ','
            << b->getTitle() << ",Yes\n";
    }

    // Then, if requested, list checked‑out books as unavailable
    if (!onlyAvailable) {
        for (auto &p : checkedOut) {
            Book* b = dynamic_cast<Book*>(p.second.get());
            if (!b) continue;
            ofs << b->getId() << ','
                << b->getAuthor() << ','
                << b->getGenre() << ','
                << b->getTitle() << ",No\n";
        }
    }
}

// Save current catalog to a binary file for efficient storage
void FileIO::saveCatalogBinary(const string &filename,
    const unordered_map<int, unique_ptr<LibraryItem>>& catalog)
{
    ofstream ofs(filename, ios::binary);
    if (!ofs) throw runtime_error("Could not open " + filename + " for binary write");

    // write count
    size_t count = catalog.size();
    ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // then write each Book’s data by down-casting
    for (auto &p : catalog) {
        Book* b = dynamic_cast<Book*>(p.second.get());
        if (!b) continue;  // skip non-Book items

        // write integer ID
        int id = b->getId();
        ofs.write(reinterpret_cast<const char*>(&id), sizeof(id));

        // helper for strings
        auto writeString = [&](const string &s){
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        };

        // write author, title, genre
        writeString(b->getAuthor());
        writeString(b->getTitle());
        writeString(b->getGenre());
    }
}


// Load catalog from a binary file on startup
void FileIO::loadCatalogBinary(const string &filename,
    unordered_map<int, unique_ptr<LibraryItem>> &catalog)
{
    ifstream ifs(filename, ios::binary);
    if (!ifs)
        return;   // no file = seed defaults

    // skip zero‑length files
    if (std::filesystem::file_size(filename) == 0)
        return;

    size_t count = 0;
    ifs.read(reinterpret_cast<char*>(&count), sizeof(count));
    if (ifs.fail())
        return;   // silently give up

    for (size_t i = 0; i < count; ++i) {
        int id;
        ifs.read(reinterpret_cast<char*>(&id), sizeof(id));
        if (ifs.fail())
            return;  // silent bad‑format exit

        // helper to read strings
        auto readString = [&](string &s){
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            if (ifs.fail()) return false;
            s.resize(len);
            ifs.read(&s[0], len);
            return !ifs.fail();
        };

        string author, title, genre;
        if (!readString(author) || !readString(title) || !readString(genre))
            return;

        // insert back (overwrites seeded items with same ID)
        catalog[id] = make_unique<Book>(id, author, title, genre);
    }
}

