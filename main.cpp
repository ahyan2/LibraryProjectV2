#include "../include/my_utils.h"
#include "../include/library.h"
#include "../include/file_io.h"

#include <iostream> // for print

using namespace std;


int main() {
    loadCatalogBinary("catalog.bin");
    displayWelcome();
    displayBooks();
    userCatalogInteraction();

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
