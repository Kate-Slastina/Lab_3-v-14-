#pragma once
#include "Exceptions.hpp"
#include <algorithm>

template <class T>
class DynamicArray {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

public:
    DynamicArray(size_t size = 0);
    DynamicArray(T* items, size_t count);
    DynamicArray(const DynamicArray<T>& other);
    DynamicArray(DynamicArray&& other);
    ~DynamicArray() { delete[] data_; }

    DynamicArray& operator=(const DynamicArray& other);
    DynamicArray& operator=(DynamicArray&& other);

    T Get(size_t index) const;
    size_t GetSize() const { return size_; }
    void Set(size_t index, T value);
    void Append(T value);
    void Resize(size_t newSize);
    void EnsureCapacity(size_t minCapacity);
};

#include "DynamicArray.tpp"