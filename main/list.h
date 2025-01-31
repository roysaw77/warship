#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>  // For exceptions
using namespace std;

template <typename T>
class list {
private:
    // Node structure
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

public:
    // Constructor
    list() : head(nullptr), tail(nullptr), count(0) {}

    // Destructor
    ~list() {
        clear();
    }

    // Copy constructor
    list(const list& other) : head(nullptr), tail(nullptr), count(0) {
        Node* current = other.head;
        while (current) {
            push_back(current->data);
            current = current->next;
        }
    }

    // Copy assignment operator
    list& operator=(const list& other) {
        if (this != &other) {
            clear();
            Node* current = other.head;
            while (current) {
                push_back(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Move constructor
    list(list&& other) noexcept 
        : head(other.head), tail(other.tail), count(other.count) {
        other.head = other.tail = nullptr;
        other.count = 0;
    }

    // Move assignment operator
    list& operator=(list&& other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            count = other.count;
            other.head = other.tail = nullptr;
            other.count = 0;
        }
        return *this;
    }

    // Add element to the end of the list
    void push_back(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    // Add element to the beginning of the list
    void push_front(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        count++;
    }

    // Remove element by value
    void erase(const T& value) {
        if (empty()) return;

        // If the head node matches the value
        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            count--;
            if (empty()) tail = nullptr;
            return;
        }

        // Search for the node to delete
        Node* current = head;
        while (current->next && current->next->data != value) {
            current = current->next;
        }

        // If the node is found, delete it
        if (current->next) {
            Node* temp = current->next;
            current->next = current->next->next;
            delete temp;
            count--;
            if (!current->next) tail = current; // Update tail if last node is deleted
        }
    }

    // Erase element by iterator
    class Iterator {
private:
    Node* current;
    Node* previous;
    list<T>* parentList;

public:
    Iterator(Node* node, list<T>* parent) : current(node), parentList(parent), previous(nullptr) {}

    // Getter functions for private members
    Node* getCurrentNode() const { return current; }
    list<T>* getParentList() const { return parentList; }

    // Dereference operator
    T& operator*() const {
        return current->data;
    }

    // Pre-increment operator
    Iterator& operator++() {
        if (current) {
            previous = current;
            current = current->next;
        }
        return *this;
    }

    // Inequality operator
    bool operator!=(const Iterator& other) const {
        return current != other.current;
    }
};


  Iterator erase(Iterator it) {
    if (!it.getCurrentNode()) {
        // Return an invalid iterator when the input iterator is invalid
        return Iterator(nullptr, it.getParentList()); 
    }

    Node* nodeToDelete = it.getCurrentNode();
    Node* nextNode = nodeToDelete->next;

    // If deleting the head node
    if (nodeToDelete == it.getParentList()->head) {
        it.getParentList()->head = nextNode;
        if (!it.getParentList()->head) it.getParentList()->tail = nullptr;  // If list is empty
    } else {
        // Find the previous node
        Node* prev = it.getParentList()->head;
        while (prev && prev->next != nodeToDelete) {
            prev = prev->next;
        }
        if (prev) prev->next = nextNode;

        // If deleting the tail node
        if (nodeToDelete == it.getParentList()->tail) {
            it.getParentList()->tail = prev;
        }
    }

    delete nodeToDelete;
    it.getParentList()->count--;

    return Iterator(nextNode, it.getParentList());  // Return the new valid iterator
}


Iterator insert(Iterator it, const T& value) {
    if (!it.getCurrentNode()) return it;  // Invalid iterator

    Node* newNode = new Node(value);
    newNode->next = it.getCurrentNode()->next;
    it.getCurrentNode()->next = newNode;

    if (it.getCurrentNode() == tail) {
        tail = newNode;  // Update tail if inserted at the end
    }

    count++;

    return Iterator(newNode, this);  // Return iterator to the new node
}


    // Get size of the list
    size_t size() const {
        return count;
    }

    // Check if the list is empty
    bool empty() const {
        return count == 0;
    }

    // Clear the list
    void clear() {
        while (!empty()) {
            Node* temp = head;
            head = head->next;
            delete temp;
            count--;
        }
        tail = nullptr;
    }

    // Get iterator to first element
    Iterator begin() {
        return Iterator(head, this);
    }

    // Get iterator to past-the-end element
    Iterator end() {
        return Iterator(nullptr, this);
    }
};

#endif // LIST_H
