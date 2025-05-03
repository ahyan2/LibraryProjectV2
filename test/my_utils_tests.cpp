#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <sstream>
#include "../include/my_utils.h"

using namespace std;

// Helper to read an entire text file into a single string
// Opens `fn` file to read, uses stream buffer to slurp contents
// Returns an empty string if the file cannot be opened.
static string slurp(const string &fn) {
    ifstream ifs(fn);
    ostringstream ss;
    ss << ifs.rdbuf();              // read the entire file buffer into ss
    return ss.str();                // return accumulated string
}

// Captures standard output from `fn`.
// `fn()` may write to cout.
//  testing functions that print to console
static string captureOutput(function<void()> fn) {
    ostringstream oss;                          // buffer to capture output
    auto* oldBuf = cout.rdbuf(oss.rdbuf());     // redirect cout to oss
    fn();            // function test
    cout.rdbuf(oldBuf);       // restore original cout buffer
    return oss.str();       // return captured output
}


// rejects non‐numeric and negative inputs, then accepts a correct one
TEST(ValidIDEdge, RejectsBadThenAcceptsGood) {
    // user typing "abc" , then "-123" , then "11111"
    istringstream in("abc\n-123\n11111\n");
    auto* oldBuf = cin.rdbuf(in.rdbuf());  // Redirect cin to read from our string
    cin.clear();                           // Clear any error flags before calling validID

    // validID should ignore the bad inputs and finally return 11111
    EXPECT_EQ(validID(5), 11111);

    // Restore original cin buffer and state so later tests aren’t affected
    cin.rdbuf(oldBuf);
    cin.clear();
}

// hen no book checked out
TEST(CheckInEdge, NoBookReturnsFalse) {
    studentID = 123;
    checkedOutBooks.clear();
    EXPECT_FALSE(checkIn());
}

// checkIn() prints a message when nothing to check in
TEST(CheckInOutput, PrintsNoBookMessage) {
    studentID = 500;
    checkedOutBooks.clear();  // ensure empty

    // Capture whatever checkIn() prints to cout
    string output = captureOutput([](){
        checkIn();
    });

    // It should mention “No books to check in.”
    EXPECT_NE(output.find("No books to check in."), string::npos);
}

// test checkOut()/checkIn() single‑book rule and return
TEST(CheckoutCheckin, SingleBookPerUserAndReturn) {
    studentID = 42;
    catalog.clear();
    checkedOutBooks.clear();

    const int BOOK1 = 11111;
    const int BOOK2 = 22222;
    catalog[BOOK1] = Book{BOOK1, "AuthorA", "TitleA", "GenreA"};

    // Checkout BOOK1
    {
        istringstream in("11111\ny\n");
        auto* oldCin = cin.rdbuf(in.rdbuf());
        cin.clear();
        checkOut();
        cin.rdbuf(oldCin);
        cin.clear();
    }
    EXPECT_TRUE(checkedOutBooks.count(42));
    EXPECT_FALSE(catalog.count(BOOK1));

    // Attempt second checkout (rejected)
    catalog[BOOK2] = Book{BOOK2, "AuthorB", "TitleB", "GenreB"};
    {
        istringstream in("22222\ny\n");
        auto* oldCin = cin.rdbuf(in.rdbuf());
        cin.clear();
        checkOut();
        cin.rdbuf(oldCin);
        cin.clear();
    }
    EXPECT_EQ(checkedOutBooks.size(), 1u);

    // Check back in
    EXPECT_TRUE(checkIn());
    EXPECT_TRUE(catalog.count(BOOK1));
    EXPECT_FALSE(checkedOutBooks.count(42));
}

// empty + basic ordering sort
TEST(SortFunctions, EmptyAndBasicOrder) {
    catalog.clear();
    EXPECT_TRUE(getCatalogSortedByTitle().empty());
    EXPECT_TRUE(getCatalogSortedByAuthor().empty());

    catalog[1] = Book{1, "Zed",  "ZTitle", "G"};
    catalog[2] = Book{2, "Amy",  "ATitle", "G"};
    auto byTitle  = getCatalogSortedByTitle();
    auto byAuthor = getCatalogSortedByAuthor();
    EXPECT_EQ(byTitle[0].title,  "ATitle");
    EXPECT_EQ(byAuthor[0].author, "Amy");
}

// when onlyAvailable=true, checked‑out books should be excluded
TEST(CSVExportEdge, OnlyAvailableExcludesCheckedOut) {
    catalog.clear();                 // start with empty catalog
    checkedOutBooks.clear();         // no user has any book
    catalog[10] = Book{10, "A", "A", "G"};     // available book
    checkedOutBooks[999] = make_unique<Book>(Book{20, "B", "B", "G"}); // simulate a checked‑out book

    // Export only available books
    exportCatalog("only_available.csv", true);
    string csv = slurp("only_available.csv");

    // The available book (ID 10) must appear
    EXPECT_NE(csv.find("10,A,G,A,Yes"), string::npos);
    // The checked‑out book (ID 20) must NOT appear
    EXPECT_EQ(csv.find("20,B,G,B,No"), string::npos);

    remove("only_available.csv");   // cleanup
}

// verify header line and at least one data line
TEST(CSVExport, HeaderAndContents) {
    catalog.clear();
    checkedOutBooks.clear();
    catalog[3] = Book{3, "Auth", "Tit", "Gen"};  // single entry

    exportCatalog("test.csv", false);
    string csv = slurp("test.csv");

    // The CSV header must be present
    EXPECT_NE(csv.find("ID,Author,Genre,Title,Available"), string::npos);
    // The book line must list ID=3, Author=Auth, Genre=Gen, Title=Tit, Available=Yes
    EXPECT_NE(csv.find("3,Auth,Gen,Tit,Yes"), string::npos);

    remove("test.csv");             // cleanup
}

// feedback appending: file should contain all lines appended
TEST(Feedback, AppendsSuccessfully) {
    remove("feedback.txt");         // ensure fresh start
    saveFeedback("LineOne");        // first append
    saveFeedback("LineTwo");        // second append
    string fb = slurp("feedback.txt");

    // Both lines should appear somewhere in the file
    EXPECT_NE(fb.find("LineOne"), string::npos);
    EXPECT_NE(fb.find("LineTwo"), string::npos);

    remove("feedback.txt");         // cleanup
}

// binary file, saving and loading should preserve all fields
TEST(BinaryIOEdge, RoundTripPreservesData) {
    catalog.clear();
    // Add a book
    catalog[7] = Book{7, "XAuth", "XTitle", "XGenre"};

    saveCatalogBinary("test.bin");   // write to binary
    catalog.clear();                 // clear in‑memory catalog
    loadCatalogBinary("test.bin");   // reload from binary

    // After reload, the book with ID=7 must exist with identical fields
    EXPECT_TRUE(catalog.count(7));
    EXPECT_EQ(catalog[7].author, "XAuth");
    EXPECT_EQ(catalog[7].title,  "XTitle");
    EXPECT_EQ(catalog[7].genre,  "XGenre");

    remove("test.bin");              // cleanup
}
