#include "../include/my_utils.h"
#include "../include/Library.h"
#include "../include/file_io.h"

#include <iostream> // for print

using namespace std;


int main() {
    Library lib;
    loadCatalogBinary("catalog.bin");
    lib.displayWelcome();
    lib.displayBooks();
    lib.userCatalogInteraction();

    // Prompt for feedback
    cout << "\nPlease provide feedback on our library service:\n> ";
    string fb;
    getline(cin, fb);
    saveFeedback(fb);

    // Export full printable list
    exportCatalog("library_export.csv", false);

    // Save state
    saveCatalogBinary("catalog.bin");

    return 0;
}
