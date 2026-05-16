
#pragma once
#include <Exceptions.hpp>

template <class T>
class DynamicArray {
private:
    T* data_;
    size_t size_;
public:
    DynamicArray(size_t size);
    DynamicArray(T* items, size_t count);
    DynamicArray(const DynamicArray<T>& other);
    ~DynamicArray() { delete[] data_; }
    
    T Get(size_t index) const;
    size_t GetSize() const { return size_; }
    void Set(size_t index, T value);
    void Resize(size_t newSize);
};

#include "DynamicArray.tpp"