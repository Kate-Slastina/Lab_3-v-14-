#include <algorithm>

template <class T>
DynamicArray<T>::DynamicArray(size_t size) : size_(size), capacity_(size) {
    data_ = new T[capacity_]();
}

template <class T>
DynamicArray<T>::DynamicArray(T* items, size_t count) : size_(count), capacity_(count) {
    data_ = new T[capacity_];
    std::copy(items, items + count, data_);
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) 
    : size_(other.size_), capacity_(other.capacity_) {
    data_ = new T[capacity_];
    std::copy(other.data_, other.data_ + size_, data_);
}

template<class T>
DynamicArray<T>::DynamicArray(DynamicArray&& other)
    : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
}

template<class T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray& other) {
    if (this != &other) {
        delete[] data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = new T[capacity_];
        std::copy(other.data_, other.data_ + size_, data_);
    }
    return *this;
}

template<class T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray&& other) {
    if (this != &other) {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }
    return *this;
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
void DynamicArray<T>::Append(T value) {
    EnsureCapacity(size_ + 1);
    data_[size_++] = value;
}

template<class T>
void DynamicArray<T>::Resize(size_t newSize) {
    EnsureCapacity(newSize);
    size_ = newSize;
}

template<class T>
void DynamicArray<T>::EnsureCapacity(size_t minCapacity) {
    if (minCapacity <= capacity_) return;
    size_t newCap = capacity_ == 0 ? 1 : capacity_;
    while (newCap < minCapacity) newCap *= 2;
    T* newData = new T[newCap]();
    std::copy(data_, data_ + size_, newData);
    delete[] data_;
    data_ = newData;
    capacity_ = newCap;
}