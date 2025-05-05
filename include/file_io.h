#ifndef FILE_IO_H
#define FILE_IO_H

#include "library.h"  // for Book, catalog, checkedOutBooks

void saveFeedback(const string &feedback);  // Append user feedback to a text file

void exportCatalog(const string &filename, bool onlyAvailable); // Export catalog to CSV/TXT (all or only available)

void saveCatalogBinary(const string &filename);     // Save current catalog to a binary file

void loadCatalogBinary(const string &filename);

#endif //FILE_IO_H
