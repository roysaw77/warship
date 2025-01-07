#ifndef CUSTOM_VECTOR_H
#define CUSTOM_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <algorithm>

template <typename T>
class CustomVector {
private:
    size_t size;
    size_t capacity;
    T *data;

    void resize() {
        if(size == capacity) {
            capacity = capacity == 0 ? 1 : capacity * 2;
            T *new_data = new T[capacity];
            for(size_t i = 0; i < size; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
        }
    }

public:
    CustomVector() : data(nullptr), size(0), capacity(0) {}

    CustomVector(const CustomVector& other) : size(other.size), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    CustomVector(std::initializer_list<T> init) : size(init.size()), capacity(init.size()) {
        data = new T[capacity];
        std::copy(init.begin(), init.end(), data);
    }
    
    CustomVector& operator=(const CustomVector& other) {
        if (this != &other) {
            delete[] data;
            size = other.size;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~CustomVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if(size == capacity) {
            resize();
        }
        data[size++] = value;
    }

    T& operator[](size_t index) {
        return data[index]; 
    }

    const T& operator[](size_t index) const {
        return data[index]; 
    }

    size_t length() const {
        return size;
    }

    size_t get_capacity() const {
        return capacity;
    }

    void clear() {
        size = 0;
    }
};

#endif 
