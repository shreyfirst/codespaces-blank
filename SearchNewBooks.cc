/* 

functions to implement:
- prompt the user to choose search method (main)
    - linear search
    - binary search
    - recursive binary search
- class for determining CPU time 

*/

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include <chrono> // NOLINT (build/c++11)
#include <cmath> 

using namespace std;

class Book {
    public:
        Book(std::string isbn, std::string lang, std::string type)
            : isbn_(isbn), language_(lang), type_(type) {}

        std::string isbn() const { return isbn_; }
        std::string language() const { return language_; }
        std::string type() const { return type_; }

        friend std::ostream& operator<<(std::ostream& os, const Book& book) {
            os << "Book ISBN: " << book.isbn_ << " Language: " << book.language_
            << " Type: " << book.type_;
            return os;
        }

    bool operator<(const Book& other) const {
        if (isbn_ != other.isbn_) {
            return isbn_ < other.isbn_;
        }
        if (type_ != other.type_) {
            if(type_ == "new") return true;
            if(other.type_ == "new") return false;
            if(type_ == "used") return true;
            if(other.type_ == "used") return false;
        }
        return language_ < other.language_;
    }
    bool operator==(const Book& other) const {
        return isbn_ == other.isbn_ && language_ == other.language_ && type_ == other.type_;
    }

    private:
        std::string isbn_;
        std::string language_;
        std::string type_;

};

int searchBooksBinary(std::vector<Book>& newBooks, std::vector<Book>& requestBooks) {
    int count = 0;
    for (auto& book : requestBooks) {
        auto it = std::lower_bound(newBooks.begin(), newBooks.end(), book);
        if (it != newBooks.end() && *it == book) {
            count ++;
            // std::cout << "Book found: " << book << std::endl;
        } else {
            // std::cout << "Book not found: " << book << std::endl;
        }
    }
    return count;
}

std::vector<Book>::iterator searchBooksBinaryRecursive(std::vector<Book>& books, const Book& book, int left, int right) {
    if (left > right) {
        return books.end();
    }
    int mid = left + (right - left) / 2;
    if (books[mid] == book) {
        return books.begin() + mid;
    }
    else if (books[mid] < book) {
        return searchBooksBinaryRecursive(books, book, mid + 1, right);
    }
    else {
        return searchBooksBinaryRecursive(books, book, left, mid - 1);
    }
}

int searchBooksLinear(std::vector<Book>& newBooks, std::vector<Book>& requestBooks) {
    int count = 0;
    for (auto& book : requestBooks) {
        bool found = false;
        for (auto& newBook : newBooks) {
            if (book == newBook) {
                found = true;
                break;
            }
        }
        if (found) {
            count ++;
            // std::cout << "Book found: " << book << std::endl;
        } else {
            // std::cout << "Book not found: " << book << std::endl;
        }
    }
    return count;
}


std::vector<Book> readBooksFromFile(const std::string& fileName) {
    std::vector<Book> books;
    std::ifstream file(fileName);
    std::string line;

    while (std::getline(file, line)) {
        std::string isbn, lang, type;
        std::stringstream ss(line);

        std::getline(ss, isbn, ',');
        std::getline(ss, lang, ',');
        std::getline(ss, type);

        books.push_back(Book(isbn, lang, type));
    }

    return books;
}
class ElapsedTime {
    public:
        ElapsedTime() : start_(std::chrono::high_resolution_clock::now()) {}

        void start() {
            start_ = std::chrono::high_resolution_clock::now();
        }

        double elapsed() const {
            auto end = std::chrono::high_resolution_clock::now();
            double elapsed_us = std::chrono::duration<double, std::micro>(end - start_).count();
            return elapsed_us;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};



int main(int argc, char *argv[]) {
    std::vector<Book> newBooks, requestBooks;
    string newBooksFile_n, requestBooksFile_n, resultBooksFile_n;
    
    if (argc < 3 || argc > 4) {
        std::cerr << "Usage: " << argv[0] << " <newBooks.dat> <request.dat> <result_file.dat>" << std::endl;
        return 1;
    } else {
        newBooksFile_n = argv[1];
        requestBooksFile_n = argv[2];
    }

    if (argc == 4) {
        resultBooksFile_n = argv[3];     
    } else {
        resultBooksFile_n = "found.dat";
    }
    
    try {
        newBooks = readBooksFromFile(newBooksFile_n);
        requestBooks = readBooksFromFile(requestBooksFile_n);
    } catch (const std::exception& e) {
        std::cerr << "Error: cannot open file " << newBooksFile_n << " or "<< requestBooksFile_n << std::endl;
        return 1;
    }

    // cout << "You entered: " << newBooksFile_n << requestBooksFile_n << resultBooksFile_n << endl;
    
    char choice;
    do {
        std::cout << "Choice of search method ([l]inear, [b]inary, [r]ecursiveBinary)? ";
        std::cin >> choice;
        if (choice != 'l' && choice != 'b' && choice != 'r') {
            std::cerr << "Incorrect choice" << std::endl;
        }
    } while (choice != 'l' && choice != 'b' && choice != 'r');

    // std::cout << choice << std::endl;
    
    newBooks = readBooksFromFile(newBooksFile_n);
    requestBooks = readBooksFromFile(requestBooksFile_n);

    ElapsedTime timer;

    int res = 0;

    if (choice == 'l') {
        timer.start();
        res = searchBooksLinear(newBooks, requestBooks);
    } else if (choice == 'b') {
        std::sort(newBooks.begin(), newBooks.end());
        timer.start();
        res = searchBooksBinary(newBooks, requestBooks);
    } else if (choice == 'r') {
        std::sort(newBooks.begin(), newBooks.end());
        timer.start();
        int count = 0;
        for (auto& book : requestBooks) {
            auto it = searchBooksBinaryRecursive(newBooks, book, 0, newBooks.size()-1);
            if (it != newBooks.end()) {
                count++;
                // std::cout << "Book found: " << book << std::endl;
            } else {
                // std::cout << "Book not found: " << book << std::endl;
            }
        }
        res = count;
    }
    double time = timer.elapsed();
    std::cout << res << std::endl;
    std::cout << "CPU time: " << time << " microseconds" << std::endl;

    std::ofstream out_file(resultBooksFile_n);
    if (out_file.is_open()) {
        out_file << res;
        out_file.close();
    }

    return 0;
}
