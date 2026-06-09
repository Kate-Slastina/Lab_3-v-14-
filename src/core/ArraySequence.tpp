#include "LinkedList.hpp"

template<class T>
ArraySequence<T>::ArraySequence(T* items, size_t count) {
    data_ = new DynamicArray<T>(items, count);
}

template<class T>
ArraySequence<T>::ArraySequence(const LinkedList<T>& list) {
    size_t length = list.GetLength();
    data_ = new DynamicArray<T>(length);
    for (size_t i = 0; i < length; ++i) {
        data_->Set(i, list.Get(i));
    }
}

template<class T>
ArraySequence<T>::ArraySequence(const ArraySequence<T>& other) {
    data_ = new DynamicArray<T>(*other.data_);
}

template<class T>
Sequence<T>* ArraySequence<T>::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (endIndex >= data_->GetSize() || startIndex > endIndex) {
        throw IndexOutOfRangeException("ArraySequence", "GetSubsequence", "invalid range.");
    }
    size_t len = endIndex - startIndex + 1;
    T* buffer = new T[len];
    for (size_t i = 0; i < len; ++i) {
        buffer[i] = data_->Get(startIndex + i);
    }
    Sequence<T>* result = this->Clone();
    delete static_cast<ArraySequence<T>*>(result)->data_;
    static_cast<ArraySequence<T>*>(result)->data_ = new DynamicArray<T>(buffer, len);
    delete[] buffer;
    return result;
}

template<class T>
void ArraySequence<T>::AppendInternal(T item) {
    data_->Append(item);
}

template<class T>
Sequence<T>* ArraySequence<T>::Append(T item) {
    Sequence<T>* target = this->Instance();
    static_cast<ArraySequence<T>*>(target)->AppendInternal(item);
    return target;
}

template<class T>
void ArraySequence<T>::PrependInternal(T item) {
    size_t old = data_->GetSize();
    data_->Resize(old + 1);
    for (size_t i = old; i > 0; --i) {
        data_->Set(i, data_->Get(i-1));
    }
    data_->Set(0, item);
}

template<class T>
Sequence<T>* ArraySequence<T>::Prepend(T item) {
    Sequence<T>* target = this->Instance();
    static_cast<ArraySequence<T>*>(target)->PrependInternal(item);
    return target;
}

template<class T>
void ArraySequence<T>::InsertAtInternal(T item, size_t index) {
    size_t old = data_->GetSize();
    if (index > old) {
        throw IndexOutOfRangeException("ArraySequence", "InsertAtInternal", "index > size");
    }
    data_->Resize(old + 1);
    for (size_t i = old; i > index; --i) {
        data_->Set(i, data_->Get(i-1));
    }
    data_->Set(index, item);
}

template<class T>
Sequence<T>* ArraySequence<T>::InsertAt(T item, size_t index) {
    Sequence<T>* target = this->Instance();
    static_cast<ArraySequence<T>*>(target)->InsertAtInternal(item, index);
    return target;
}

template<class T>
void ArraySequence<T>::ConcatInternal(Sequence<T>* smth) {
    if (smth == nullptr) throw InvalidArgumentException("ArraySequence", "Concat", "null sequence");
    size_t old = data_->GetSize();
    size_t otherSize = smth->GetLength();
    if (otherSize == 0) return;
    data_->Resize(old + otherSize);
    for (size_t i = 0; i < otherSize; ++i) {
        data_->Set(old + i, smth->Get(i));
    }
}

template<class T>
Sequence<T>* ArraySequence<T>::Concat(Sequence<T>* smth) {
    Sequence<T>* target = this->Clone();
    static_cast<ArraySequence<T>*>(target)->ConcatInternal(smth);
    return target;
}

template<class T>
Sequence<T>* ArraySequence<T>::RemoveAt(size_t index) {
    if (index >= data_->GetSize())
        throw IndexOutOfRangeException("ArraySequence", "RemoveAt", "index out of range");
    Sequence<T>* target = this->Instance();
    ArraySequence<T>* arr = static_cast<ArraySequence<T>*>(target);
    size_t oldSize = arr->data_->GetSize();
    auto* newData = new DynamicArray<T>(oldSize - 1);
    for (size_t i = 0, j = 0; i < oldSize; ++i) {
        if (i != index) {
            newData->Set(j++, arr->data_->Get(i));
        }
    }
    delete arr->data_;
    arr->data_ = newData;
    return target;
}