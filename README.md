Overview: A C++ interactive library catalog that uses advanced data structures, 
smart pointers, and file I/O to let users browse, check out/in books, view overviews, save state, 
export data, and leave feedback.


Features:

1. User Menu (via enum UserAction)
- Check out a book (5‑digit ID)
- Check in a book
- View the full library catalog
- View spoiler‑free overviews (from .txt files)
- Leave the program (cannot exit while a book is checked out)
- Sort catalog by title
- Sort catalog by author

2. Data Structures & Memory Management
- struct Book { id, author, title, genre }
- unordered_map<int,Book> catalog for fast lookup by ID
- unordered_map<int, unique_ptr<Book>> checkedOutBooks to enforce one‑book‑per‑user and automatic cleanup

3. Input Validation Helpers
- validID(int) loops until N‑digit positive integer is entered
- validFirstName() rejects empty or non‑alphabetic names
- validChoice() enforces ‘y’/’n’ responses

4. File I/O
- Binary State: loadCatalogBinary("catalog.bin") on startup, saveCatalogBinary("catalog.bin") on exit
- Text CSV Export: exportCatalog("library_export.csv", onlyAvailable=false) writes all books (available/unavailable)
- Feedback Log: saveFeedback(string) appends user feedback to feedback.txt
- Overview Files: Book overviews stored as <ID>.txt and displayed via displayOverview()

5. Testing (GoogleTest)
- Unit tests for all core functions: validID(), checkOut(), checkIn(), sort, CSV export, binary I/O, feedback logging
- Edge cases ensure no infinite loops or crashes on bad input
- Integrated via CMake


Video Demo:
https://drive.google.com/drive/folders/1ZHto2Tbn4cY1DwgWf6CEFoGdlQoc1fg1


// Sources & Learning
- GeeksforGeeks
- StackOverflow
- C++ documentation
- GoogleTest framework