
#include <algorithm>

template <class T>
DynamicArray<T>::DynamicArray(size_t size) : size_(size) {
    data_ = new T[size_];
}

template <class T>
DynamicArray<T>::DynamicArray(T* items, size_t count) : size_(count) {
    data_ = new T[size_];
    std::copy(items, items + size_, data_);
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) : size_(other.size_) {
    data_ = new T[size_];
    std::copy(other.data_, other.data_ + size_, data_);
}

template<class T>
T DynamicArray<T>::Get(size_t index) const {
    if (index >= size_) {
        throw IndexOutOfRangeException("DynamicArray", "Get", "index out of bounds.");
    }
    return data_[index];
}

template<class T>
void DynamicArray<T>::Set(size_t index, T value) {
    if (index >= size_) {
        throw IndexOutOfRangeException("DynamicArray", "Set", "index out of bounds.");
    }
    data_[index] = value;
}

template<class T>
void DynamicArray<T>::Resize(size_t newSize) {
    T* newData = new T[newSize]();
    size_t copyCount = (newSize < size_) ? newSize : size_;
    for (size_t i = 0; i < copyCount; ++i) {
        newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    size_ = newSize;
}