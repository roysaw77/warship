#ifndef CVECTOR_H
#define CVECTOR_H

#include <cstddef>

template <typename T>
class CVector {
private:
    size_t _size;       // Renamed from size to _size
    size_t _capacity;   // Renamed from capacity to _capacity
    T *data;

    void resize() {
        if (_size == _capacity) {
            _capacity = _capacity == 0 ? 1 : _capacity * 2;
            T *new_data = new T[_capacity];
            for (size_t i = 0; i < _size; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
        }
    }

public:
    CVector() : data(nullptr), _size(0), _capacity(0) {}

    CVector(const CVector& other) : _size(other._size), _capacity(other._capacity) {
        data = new T[_capacity];
        for (size_t i = 0; i < _size; ++i) {
            data[i] = other.data[i];
        }
    }

    CVector(CVector&& other) : data(other.data), _size(other._size), _capacity(other._capacity) {
        other.data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    CVector& operator=(const CVector& other) {
        if (this != &other) {
            delete[] data;
            _size = other._size;
            _capacity = other._capacity;
            data = new T[_capacity];
            for (size_t i = 0; i < _size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~CVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (_size == _capacity) {
            resize();
        }
        data[_size++] = value;
    }

    T& operator[](size_t index) {
        return data[index]; 
    }

    const T& operator[](size_t index) const {
        return data[index]; 
    }

    size_t size() const { // No naming conflict here
        return _size;
    }

    size_t get_capacity() const {
        return _capacity;
    }

    void clear() {
        _size = 0;
    }

    T* begin() { return data; }
    T* end() { return data + _size; }
    const T* begin() const { return data; }
    const T* end() const { return data + _size; }
};

#endif // CVECTOR_H