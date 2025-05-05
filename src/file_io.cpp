#include "../include/file_io.h"
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

using namespace std;


// Append user feedback to a text file
void saveFeedback(const string &feedback) {
    // Open feedback.txt in append mode so we don’t overwrite existing feedback
    ofstream ofs("feedback.txt", ios::app);
    if (!ofs) {
        cout << "Error: Could not open feedback.txt for writing.\n";
        return;  // Early return on file open failure
    }
    ofs << feedback << "\n";  // Write the user's feedback, followed by a newline
}

// Export catalog to CSV/TXT
void exportCatalog(const string &filename, bool onlyAvailable) {
    // Open the given filename for text output (overwrites existing file)
    ofstream ofs(filename);
    if (!ofs) {
        cout << "Error: Could not open " << filename << " for writing.\n";
        return;  // Abort if file can't be created
    }

    // Write a header row for CSV format
    ofs << "ID,Author,Genre,Title,Available\n";

    // First, list all books still in the catalog (available)
    for (auto &p : catalog) {
        const Book &b = p.second;
        ofs << b.id << ','
            << b.author << ','
            << b.genre << ','
            << b.title << ",Yes\n";
    }

    // Then, if requested, list checked‑out books as unavailable
    if (!onlyAvailable) {
        for (auto &p : checkedOutBooks) {
            const Book &b = *p.second;
            ofs << b.id << ','
                << b.author << ','
                << b.genre << ','
                << b.title << ",No\n";
        }
    }
}

// Save current catalog to a binary file for efficient storage
void saveCatalogBinary(const string &filename) {
    // Open file in binary mode
    ofstream ofs(filename, ios::binary);
    if (!ofs) {
        cout << "Error: Could not open " << filename << " for binary write.\n";
        return;
    }

    // First write the number of Book entries
    size_t count = catalog.size();
    ofs.write(reinterpret_cast<const char*>(&count), sizeof(count));

    // Then write each Book’s data
    for (auto &p : catalog) {
        const Book &b = p.second;

        // Write the integer ID
        ofs.write(reinterpret_cast<const char*>(&b.id), sizeof(b.id));

        // Helper to write a string: first its length, then its characters
        auto writeString = [&](const string &s){
            size_t len = s.size();
            ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
            ofs.write(s.data(), len);
        };

        // Write author, title, genre
        writeString(b.author);
        writeString(b.title);
        writeString(b.genre);
    }
}

// Load catalog from a binary file on startup
void loadCatalogBinary(const string &filename) {
    // Open file for binary read
    ifstream ifs(filename, ios::binary);
    if (!ifs) {
        // File doesn't exist yet—start with the default in-memory catalog
        return;
    }

    // Read how many Book entries were saved
    size_t count;
    ifs.read(reinterpret_cast<char*>(&count), sizeof(count));

    // Clear any existing in-memory catalog
    catalog.clear();

    // Read each saved Book back into the map
    for (size_t i = 0; i < count; ++i) {
        Book b;

        // Read the integer ID
        ifs.read(reinterpret_cast<char*>(&b.id), sizeof(b.id));

        // Helper to read a string: read length then characters
        auto readString = [&](string &s){
            size_t len;
            ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
            s.resize(len);
            ifs.read(&s[0], len);
        };

        // Read author, title, genre
        readString(b.author);
        readString(b.title);
        readString(b.genre);

        // Reinsert into the in-memory catalog
        catalog[b.id] = b;
    }
}