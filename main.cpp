#include "../include/MyUtils.h"
#include "../include/Library.h"
#include "../include/FileIO.h"

#include <iostream> // for print

using namespace std;


int main() {
    Library lib;
    lib.loadCatalogBinary("catalog.bin");
    lib.displayWelcome();
    lib.displayBooks();
    lib.userCatalogInteraction();

    // Prompt for feedback
    cout << "\nPlease provide feedback on our library service:\n> ";
    string fb;
    getline(cin, fb);
    lib.saveFeedback(fb);

    // Export full printable list
    lib.exportCatalog("library_export.csv", false);

    // Save state
    lib.saveCatalogBinary("catalog.bin");

    return 0;
}
