// test/library_tests.cpp

#include <gtest/gtest.h>
#include "../include/Book.h"
#include "../include/ReferenceBook.h"
#include "../include/Library.h"
#include <cstdio>
#include <algorithm>

// --- Book tests --------------------------------------------------------

TEST(BookTest, GettersAndSummary) {
  Book b(123, "A. Author", "My Title", "GenreX");
  EXPECT_EQ(b.getId(), 123);
  EXPECT_EQ(b.getAuthor(), "A. Author");
  EXPECT_EQ(b.getTitle(), "My Title");
  EXPECT_EQ(b.getGenre(), "GenreX");
  EXPECT_EQ(b.summary(),
            "[123] \"My Title\" by A. Author [GenreX]");
}

// --- ReferenceBook tests ----------------------------------------------

TEST(ReferenceBookTest, GettersAndSummary) {
  // note: ReferenceBook(int id, string author, string title, string topic)
  ReferenceBook r(999, "Ref Author", "Ref Title", "TopicZ");

  EXPECT_EQ(r.getId(), 999);
  EXPECT_EQ(r.getAuthor(), "Ref Author");
  EXPECT_EQ(r.getTitle(),  "Ref Title");
  EXPECT_EQ(r.getGenre(),  "TopicZ");

  // summary should use title then author and topic
  EXPECT_EQ(
    r.summary(),
    "[999] \"Ref Title\" by Ref Author (Ref on TopicZ)"
  );
}

// --- Library seeding & sorting ----------------------------------------

TEST(LibraryTest, DefaultSeedingAndSortById) {
  Library lib;
  auto list = lib.getCatalogSortedById();
  ASSERT_EQ(list.size(), 7u);

  std::vector<int> ids;
  for (auto *item : list) ids.push_back(item->getId());
  std::vector<int> want = {10001,10002,10003,10004,10005,20001,20002};
  EXPECT_EQ(ids, want);
}

TEST(LibraryTest, SortByTitleIsLexical) {
  Library lib;
  auto list = lib.getCatalogSortedByTitle();
  std::vector<std::string> sums;
  for (auto *i: list) sums.push_back(i->summary());
  auto sorted = sums;
  std::sort(sorted.begin(), sorted.end());
  EXPECT_EQ(sums, sorted);
}

TEST(LibraryTest, SortByAuthorUsesBookAuthors) {
  Library lib;
  auto list = lib.getCatalogSortedByAuthor();

  std::vector<std::string> auths;
  for (auto *i: list) {
    // downcast only to Book, skip reference books here
    if (auto *b = dynamic_cast<Book*>(i))
      auths.push_back(b->getAuthor());
  }

  auto sorted = auths;
  std::sort(sorted.begin(), sorted.end());
  EXPECT_EQ(auths, sorted);
}

// --- Library check‑in only ---------------------------------------------

TEST(LibraryTest, CheckInWhenEmptyReturnsFalse) {
  Library lib;
  // without doing any checkout, checkIn() must return false
  EXPECT_FALSE(lib.checkIn());
}

// --- FileIO round‑trip via Library API -------------------------------

TEST(FileIOTest, SaveAndReloadCatalog) {
  const char *f = "tmp_catalog.bin";
  std::remove(f);

  Library a;
  a.saveCatalogBinary(f);

  Library b;
  b.loadCatalogBinary(f);

  auto A = a.getCatalogSortedById();
  auto B = b.getCatalogSortedById();
  ASSERT_EQ(A.size(), B.size());
  for (size_t i = 0; i < A.size(); ++i)
    EXPECT_EQ(A[i]->summary(), B[i]->summary());

  std::remove(f);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
