#pragma once
#include <cstddef>
#include "ICollection.hpp"
#include "Iterator.hpp"

template <class T>
class Sequence : public ICollection<T> {
public:
    virtual ~Sequence() = default;

    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const override = 0;
    virtual size_t GetLength() const override = 0;
    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;

    virtual Sequence<T>* Append(T item) = 0;
    virtual Sequence<T>* Prepend(T item) = 0;
    virtual Sequence<T>* InsertAt(T item, size_t index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>* list) = 0;
    virtual Sequence<T>* RemoveAt(size_t index) = 0;

    virtual SequenceIterator<T> begin() const = 0;
    virtual SequenceIterator<T> end() const = 0;
};