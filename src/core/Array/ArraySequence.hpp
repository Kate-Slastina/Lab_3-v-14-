#pragma once
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include <Exceptions.hpp>

template <class T> class LinkedList;  // forward declaration

template <class T>
class ArraySequence : public Sequence<T> {
private:
    void AppendInternal(T item);
    void PrependInternal(T item);
    void InsertAtInternal(T item, size_t index);
    void ConcatInternal(Sequence<T>* smth);

protected:
    DynamicArray<T>* data_;
    virtual ArraySequence<T>* Clone() const = 0;
    virtual Sequence<T>* Instance() = 0;

public:
    ArraySequence() : data_(new DynamicArray<T>(0)) {}
    explicit ArraySequence(T* items, size_t count);
    explicit ArraySequence(const LinkedList<T>& list);
    explicit ArraySequence(const ArraySequence<T>& other);
    virtual ~ArraySequence() { delete data_; }

    T GetFirst() const override { return data_->Get(0); }
    T GetLast() const override { return data_->Get(data_->GetSize() - 1); }
    T Get(size_t index) const override { return data_->Get(index); }
    size_t GetLength() const override { return data_->GetSize(); }
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;

    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, size_t index) override;
    Sequence<T>* Concat(Sequence<T>* list) override;
    Sequence<T>* RemoveAt(size_t index) override;
};

#include "ArraySequence.tpp"