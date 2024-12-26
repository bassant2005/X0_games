//Name: Rawda Raafat Ramadan
//ID: 202310676
//this is Sheet problem 2
//GitHub link: https://github.com/Marria-m/Diff.-Problems.

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cctype>

using namespace std;

class StringSet {
private:
    vector<string> words;

    string add_only_words(const string &word) {
        string clean_word;
        for (char c : word) {
            if (isalpha(c)) {
                clean_word += tolower(c);
            }
        }
        return clean_word;
    }

    bool contains(const string &word) const {
        for (const auto &w : words) {
            if (w == word) {
                return true;
            }
        }
        return false;
    }

public:

    StringSet() {}

    StringSet(const string &filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: File could not be opened.\n";
            return;
        }

        string word;
        while (file >> word) {
            word = add_only_words(word);
            if (!word.empty() && !contains(word)) {
                words.push_back(word);
            }
        }
    }

    StringSet(istringstream &textStream) {
        string word;
        while (textStream >> word) {
            word = add_only_words(word);
            if (!word.empty() && !contains(word)) {
                words.push_back(word);
            }
        }
    }

    void add(const string &word) {
        string only_words = add_only_words(word);
        if (!only_words.empty() && !contains(only_words)) {
            words.push_back(only_words);
        }
    }

    void remove(const string &word) {
        string only_words = add_only_words(word);
        words.erase(std::remove(words.begin(), words.end(), only_words), words.end());
    }

    void clear() {
        words.clear();
    }

    int size() const {
        return words.size();
    }

    void print() const {
        for (const auto &word : words) {
            cout << word << " ";
        }
        cout << endl;
    }

    StringSet operator+(const StringSet &other) const {
        StringSet result;
        for (const auto &word : words) {
            result.add(word);
        }
        for (const auto &word : other.words) {
            result.add(word);
        }
        return result;
    }

    StringSet operator*(const StringSet &other) const {
        StringSet result;
        for (const auto &word : words) {
            if (other.contains(word)) {
                result.add(word);
            }
        }
        return result;
    }

    double similarity(const StringSet &other) const {
        StringSet intersection_set = *this * other;
        int intersection_size = intersection_set.size();

        int doc_size = this->size();
        int query_size = other.size();

        double denominator = sqrt(doc_size) * sqrt(query_size);
        return (double) intersection_size / denominator;
    }

};

bool fileExists(const string &filename) {
    ifstream file(filename);
    return file.good();
}

int A2_S8_20231067_Sheetpb2() {
    StringSet document1, document2;
    int choice1;

    cout << "Choose how to initialize the document and query sets:\n";
    cout << "1. From two files\n";
    cout << "2. From two strings\n";
    cout << "Enter your choice: ";
    cin >> choice1;

    switch (choice1) {
        case 1: {
            string docFilename, queryFilename;
            cout << "Enter document filename: ";
            cin >> docFilename;

            if (!fileExists(docFilename)) {
                cerr << "Error: Document file not found.\n";
                return 1;
            }
            document1 = StringSet(docFilename);

            cout << "Enter query filename: ";
            cin >> queryFilename;

            if (!fileExists(queryFilename)) {
                cerr << "Error: Query file not found.\n";
                return 1;
            }
            document2 = StringSet(queryFilename);
            break;
        }
        case 2: {
            string doc_string, query_string;
            cout << "Enter document string: ";
            cin.ignore();
            getline(cin, doc_string);
            istringstream docStream(doc_string);
            document1 = StringSet(docStream);

            cout << "Enter query string: ";
            getline(cin, query_string);
            istringstream query_stream(query_string);
            document2 = StringSet(query_stream);

            break;
        }
        default:
            cerr << "Invalid choice. Exiting program.\n";
            return 1;
    }

    int choice;
    do {
        cout << "\nMenu:\n";
        cout << "1. Display all words in the document\n";
        cout << "2. Display all words in the query\n";
        cout << "3. Display the union of document and query\n";
        cout << "4. Display the intersection of document and query\n";
        cout << "5. Display the size of the document set\n";
        cout << "6. Display the size of the query set\n";
        cout << "7. Display similarity between document and query\n";
        cout << "8. Add a word to the first document set\n";
        cout << "9. Remove a word from the first document set\n";
        cout << "10. Clear all words from the first document set\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Document words: ";
                document1.print();
                break;
            case 2:
                cout << "Query words: ";
                document2.print();
                break;
            case 3: {
                StringSet union_set = document1 + document2;
                cout << "Union of document and query: ";
                union_set.print();
                break;
            }
            case 4: {
                StringSet intersection_set = document1 * document2;
                cout << "Intersection of document and query: ";
                intersection_set.print();
                break;
            }
            case 5:
                cout << "Size of document set: " << document1.size() << endl;
                break;
            case 6:
                cout << "Size of query set: " << document2.size() << endl;
                break;
            case 7:
                cout << "Similarity between document and query: " << document1.similarity(document2) << endl;
                break;
            case 8: {
                string word_to_add;
                cout << "Enter a word to add to the document set: ";
                cin >> word_to_add;
                document1.add(word_to_add);
                cout << "Word added.\n";
                break;
            }
            case 9: {
                string word_to_remove;
                cout << "Enter a word to remove from the document set: ";
                cin >> word_to_remove;
                document1.remove(word_to_remove);
                cout << "Word removed (if it existed).\n";
                break;
            }
            case 10:
                document1.clear();
                cout << "Document set cleared.\n";
                break;
            case 0:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);

    return 0;
}


