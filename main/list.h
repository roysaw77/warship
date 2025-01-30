#ifndef LIST_H
#define LIST_H

#include <iostream>
using namespace std;

// Node structure as a template
template <typename T>
struct Node {
    T data;          // Data of type T
    Node* next;      // Pointer to the next node

    Node(T val) : data(val), next(nullptr) {}
};

// Linked List class as a template
template <typename T>
class list {
private:
    Node<T>* head;   // Pointer to the head node

public:
    list() : head(nullptr) {}

    // PushBack: Insert at the end
    void push_back(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = newNode;
            return;
        }
        Node<T>* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Erase: Delete a node by value
    void erase(T value) {
        if (!head) return;

        if (head->data == value) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node<T>* current = head;
        while (current->next && current->next->data != value) {
            current = current->next;
        }

        if (current->next) {
            Node<T>* temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
    }

    // Display the linked list
    void display() {
        Node<T>* temp = head;
        while (temp) {
            cout << temp->data << " -> ";
            temp = temp->next;
        }
        cout << "NULL\n";
    }

    // Destructor to free memory
    ~list() {
        Node<T>* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }
    

    
};

#endif // LIST_H