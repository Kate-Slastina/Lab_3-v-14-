
#pragma once
#include "Sequence.hpp"
#include "LinkedList.hpp"

template <class T>
class ListSequence : public Sequence<T> {
private:
    void AppendInternal(T item);
    void PrependInternal(T item);
    void InsertAtInternal(T item, size_t index);
    void ConcatInternal(Sequence<T>* smth);

protected:
    LinkedList<T>* data_;
    virtual ListSequence<T>* Clone() const = 0;
    virtual Sequence<T>* Instance() = 0;

public:
    ListSequence() : data_(new LinkedList<T>()) {}
    explicit ListSequence(T* items, size_t count) : data_(new LinkedList<T>(items, count)) {}
    explicit ListSequence(const LinkedList<T>& list) : data_(new LinkedList<T>(list)) {}
    ListSequence(const ListSequence<T>& other) : data_(new LinkedList<T>(*(other.data_))) {}
    virtual ~ListSequence() { delete data_; }

    T GetFirst() const override { return data_->GetFirst(); }
    T GetLast() const override { return data_->GetLast(); }
    T Get(size_t index) const override { return data_->Get(index); }
    size_t GetLength() const override { return data_->GetLength(); }
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override;

    Sequence<T>* Append(T item) override;
    Sequence<T>* Prepend(T item) override;
    Sequence<T>* InsertAt(T item, size_t index) override;
    Sequence<T>* Concat(Sequence<T>* smth) override;
    Sequence<T>* RemoveAt(size_t index) override;

    ListSequence<T>& operator=(const ListSequence<T>& other) {
        if (this != &other) {
            delete data_;
            data_ = new LinkedList<T>(*(other.data_));
        }
        return *this;
    }
};

#include "ListSequence.tpp"