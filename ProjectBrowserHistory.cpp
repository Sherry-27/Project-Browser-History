#include <iostream>
#include <ctime>
#include <string>
using namespace std;

struct HashTable {
    static const int TABLE_SIZE = 100; 
    string table[TABLE_SIZE]; // Array to store keys

    // Hash function to calculate the index for a given key
    int hash(string key) {
        int hashValue = 0;
        for (char c : key) {
            hashValue = (hashValue * 7 + c) % TABLE_SIZE;
        }
        return hashValue;
    }

    // Quadratic probing to handle collisions
    int quadraticProbe(int index, int attempt) {
        return (index + attempt * attempt) % TABLE_SIZE;
    }

    void add(string key) {
        int index = hash(key);
        int attempt = 1;

        while (!table[index].empty()) {
            index = quadraticProbe(index, attempt++);
        }

        table[index] = key;
    }

    bool search(const string key) {
        int index = hash(key);
        int attempt = 1;

        while (!table[index].empty()) {
            if (table[index] == key) {
                return true;
            }
            index = quadraticProbe(index, attempt++);
        }

        return false;
    }

    void remove(const string key) {
        int index = hash(key);
        int attempt = 1;

        while (!table[index].empty()) {
            if (table[index] == key) {
                table[index].clear();
                return;
            }
            index = quadraticProbe(index, attempt++);
        }
    }

    void display() {
        bool hasBookmarks = false;
        cout << "Bookmarked URLs:" << endl;
        for (int i = 0; i < TABLE_SIZE; ++i) {
            if (!table[i].empty()) {
                cout << table[i] << endl;
                hasBookmarks = true;
            }
        }

        if(!hasBookmarks){
            cout << "There are no bookmarks" << endl;
        } else {
            cout << "------------------------------" << endl;

            char choice;
            cout << "Do you want to remove any bookmark? (y/n): ";
            cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                string url;
                cout << "Enter the URL to remove from bookmarks: ";
                cin >> url;
                if (search(url)) {
                    remove(url);
                    cout << "Bookmark removed!" << endl;
                } else {
                    cout << "Bookmark not found!" << endl;
                }
            }
        }
    }
};

struct browserHistory {
    struct Node {
        string url;
        time_t timestamp;
        Node* next;
    };

    Node* head;

    browserHistory() {
        head = nullptr;
    }

    void add(const string url) {
        Node* new_Node = new Node();
        new_Node->timestamp = time(nullptr);
        new_Node->url = url;
        new_Node->next = nullptr;

        if (head == nullptr) {
            head = new_Node;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_Node;
        }
    }

    void display() {
        Node* temp = head;
        if (temp == nullptr) {
            cout << "No history available." << endl;
            return;
        }

        cout << "Visited URLs:" << endl;

        while (temp != nullptr) {
            string str_time = ctime(&temp->timestamp); // converting time to string
            str_time.pop_back(); // just used to remove \n new line character from last
            cout << temp->url << "   " << str_time << endl;
            temp = temp->next;
        }

        cout << "----------------------------------------" << endl;

        char choice;
        cout << "Do you want to delete a specific URL from history? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            string url;
            cout << "Enter the URL to delete from history: ";
            cin >> url;
            deleteUrl(url);
        }
        cout << "Do you want to delete all history? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            deleteAll();
        }
    }

    bool search(string url) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->url == url) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    void deleteUrl(string url) {
        if (head == nullptr) {
            cout << "History is empty!" << endl;
            return;
        }

        Node* current = head;
        Node* prev = nullptr;

        // Search for the URL
        while (current != nullptr && current->url != url) {
            prev = current;
            current = current->next;
        }

        // If the URL is not found
        if (current == nullptr) {
            cout << "URL not found in history!" << endl;
            return;
        }

        // If the URL is found at the head
        if (prev == nullptr) {
            head = current->next;
        } else {
            prev->next = current->next;
        }

        delete current;
        cout << "URL deleted!" << endl;
    }

    void deleteAll() {
        Node* curr = head;
        while (curr != nullptr) {
            Node* temp = curr;
            curr = curr->next;
            delete temp;
        }
        head = nullptr;
        cout << "All history deleted!" << endl;
    }
};

struct browserFunctions {
    struct Node {
        string url;
        Node* next;
        Node* prev;
    };

    Node* curr;
    browserHistory visitedURLs; // instance of history struct
    HashTable bookmarksList;    // instance of HashTable for bookmarks

    browserFunctions(const string& homepage) {
        Node* newNode = new Node();
        newNode->url = homepage;
        newNode->next = nullptr;
        newNode->prev = nullptr;
        curr = newNode;
    }

    void visitPage(const string& url) {
        if (curr == nullptr) {
            curr = new Node();
            curr->url = url;
            curr->next = nullptr;
            curr->prev = nullptr;
        } else {
            curr->next = new Node();
            curr->next->url = url;
            curr->next->prev = curr;
            curr->next->next = nullptr;
            curr = curr->next;
        }

        visitedURLs.add(url); // add method of history struct

        char choice;
        cout << "Do you want to bookmark this page? (y/n): ";
        cin >> choice;
        if (choice == 'y' || choice == 'Y') {
            bookmarksList.add(url);
            cout << "Page bookmarked!" << endl;
        }
    }

    void removeBookmark(const string& url) {
        bookmarksList.remove(url);
        cout << "Bookmark removed!" << endl;
    }

    string back(int steps) {
        while (steps != 0) {
            if (curr->prev != nullptr) {
                curr = curr->prev;
            } else {
                cout << "This is the homepage" << endl;
                break;
            }
            steps--;
        }
        return curr->url;
    }

    string forward(int steps) {
        while (steps != 0) {
            if (curr->next != nullptr) {
                curr = curr->next;
            } else {
                cout << "This is the last page you visited" << endl;
                break;
            }
            steps--;
        }
        return curr->url;
    }
};

int main() {
    browserFunctions history("homepage.com");

    while (true) {
        cout << "Choose operation: (1) Visit url, (2) Back, (3) Forward, (4) Display Bookmarks, (5) Display History,  (0) Exit: ";
        int choice;
        cin >> choice;

        if (choice == 0) {
            break;
        } else if (choice == 1) {
            string url;
            cout << "Enter URL to visit: ";
            cin >> url;
            history.visitPage(url);
            cout << "Visited: " << url << endl;
        } else if (choice == 2) {
            int steps;
            cout << "Enter number of steps to go back: ";
            cin >> steps;
            cout << "Current URL after going back: " << history.back(steps) << endl;
        } else if (choice == 3) {
            int steps;
            cout << "Enter number of steps to go forward: ";
            cin >> steps;
            cout << "Current URL after going forward: " << history.forward(steps) << endl;
        } else if (choice == 4) {
            history.bookmarksList.display();
        } else if (choice == 5) {
            history.visitedURLs.display();
        } else {
            cout << "Invalid choice. Please choose again." << endl;
        }
    }

    return 0;
}

