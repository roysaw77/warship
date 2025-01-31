#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept> // For std::runtime_error

template <typename T>
class queue {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& value) : data(value), next(nullptr) {}
    };

    Node* frontNode;
    Node* rearNode;
    size_t count;

public:
    // Constructor
    queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    // Destructor
    ~queue() {
        clear();
    }

    // Copy constructor
    queue(const queue& other) : frontNode(nullptr), rearNode(nullptr), count(0) {
        Node* current = other.frontNode;
        while (current) {
            push(current->data);
            current = current->next;
        }
    }

    // Copy assignment operator
    queue& operator=(const queue& other) {
        if (this != &other) {
            clear(); // Clear the current queue
            Node* current = other.frontNode;
            while (current) {
                push(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    // Move constructor
    queue(queue&& other) noexcept 
        : frontNode(other.frontNode), rearNode(other.rearNode), count(other.count) {
        other.frontNode = other.rearNode = nullptr;
        other.count = 0;
    }

    // Move assignment operator
    queue& operator=(queue&& other) noexcept {
        if (this != &other) {
            clear();
            frontNode = other.frontNode;
            rearNode = other.rearNode;
            count = other.count;
            other.frontNode = other.rearNode = nullptr;
            other.count = 0;
        }
        return *this;
    }

    // Add element to the end of the queue
    void push(const T& value) {
        Node* newNode = new Node(value);
        if (empty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    // Remove element from the front of the queue
    void pop() {
        if (empty()) {
            throw std::runtime_error("pop called on empty queue");
        }
        
        Node* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        count--;

        if (empty()) {
            rearNode = nullptr;
        }
    }

    // Get front element
    T& front() {
        if (empty()) {
            throw std::runtime_error("front called on empty queue");
        }
        return frontNode->data;
    }

    // Get front element (const version)
    const T& front() const {
        if (empty()) {
            throw std::runtime_error("front called on empty queue");
        }
        return frontNode->data;
    }

    // Get back element
    T& back() {
        if (empty()) {
            throw std::runtime_error("back called on empty queue");
        }
        return rearNode->data;
    }

    // Get back element (const version)
    const T& back() const {
        if (empty()) {
            throw std::runtime_error("back called on empty queue");
        }
        return rearNode->data;
    }

    // Get queue size
    size_t size() const {
        return count;
    }

    // Check if queue is empty
    bool empty() const {
        return count == 0;
    }

    // Clear the queue
    void clear() {
        while (!empty()) {
            pop();
        }
    }
};

#endif // QUEUE_H