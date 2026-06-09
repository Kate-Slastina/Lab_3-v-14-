#pragma once
#include <iostream>
#include "MutableArraySequence.hpp"
#include "Sequence.hpp"
#include <functional>
#include <utility>
#include <memory>
#include <stdexcept>

template <class T>
class Stream {
private:
    std::unique_ptr<Sequence<T>> seq_;

public:
    Stream();
    explicit Stream(Sequence<T>* seq);
    explicit Stream(std::unique_ptr<Sequence<T>> seq);
    Stream(const Stream&) = delete;
    Stream(Stream&& other) = default;
    Stream& operator=(const Stream&) = delete;
    Stream& operator=(Stream&& other) = default;

    T GetFirst() const;
    T GetLast() const;
    T Get(size_t index) const;
    size_t GetLength() const;

    template <class U>
    Stream<U> Map(const std::function<U(const T&)>& f) const;

    template <class U>
    Stream<U> MapWithIndex(const std::function<U(const T&, size_t)>& f) const;

    Stream<T> Where(const std::function<bool(const T&)>& pred) const;

    T Reduce(const std::function<T(const T&, const T&)>& f, T init) const;

    Stream<T> GetSubsequence(size_t start, size_t end) const;

    bool ContainsSubsequence(const Stream<T>& other) const;

    Stream<T> Merge(const Stream<T>& other) const;

    std::pair<Stream<T>, Stream<T>> Split(const std::function<bool(const T&)>& pred) const;

    void Print(std::ostream& os, const std::function<void(std::ostream&, const T&)>& printer = nullptr) const;

    T operator[](size_t index) const;
    Stream<T> operator+(const Stream<T>& other) const;
    Stream<T> operator+(const T& item) const;
    Stream<T>& operator+=(const T& item);
    Stream<T>& operator+=(const Stream<T>& other);
    bool operator==(const Stream<T>& other) const;
    bool operator!=(const Stream<T>& other) const;

    // ИТЕРАТОРЫ ДОЛЖНЫ БЫТЬ ЗДЕСЬ, ПЕРЕД friend
    auto begin() const { return seq_->begin(); }
    auto end() const { return seq_->end(); }

    friend std::ostream& operator<<(std::ostream& os, const Stream<T>& s) {
        s.Print(os);
        return os;
    }
};

// ------------------ РЕАЛИЗАЦИИ ------------------

template <class T>
Stream<T>::Stream() : seq_(std::make_unique<MutableArraySequence<T>>()) {}

template <class T>
Stream<T>::Stream(Sequence<T>* seq) : seq_(seq) {}

template <class T>
Stream<T>::Stream(std::unique_ptr<Sequence<T>> seq) : seq_(std::move(seq)) {}

template <class T>
T Stream<T>::GetFirst() const { return seq_->GetFirst(); }

template <class T>
T Stream<T>::GetLast() const { return seq_->GetLast(); }

template <class T>
T Stream<T>::Get(size_t index) const { return seq_->Get(index); }

template <class T>
size_t Stream<T>::GetLength() const { return seq_->GetLength(); }

template <class T> template <class U>
Stream<U> Stream<T>::Map(const std::function<U(const T&)>& f) const {
    auto newSeq = std::make_unique<MutableArraySequence<U>>();
    for (auto it = seq_->begin(); it != seq_->end(); ++it)
        newSeq->Append(f(*it));
    return Stream<U>(std::move(newSeq));
}

template <class T> template <class U>
Stream<U> Stream<T>::MapWithIndex(const std::function<U(const T&, size_t)>& f) const {
    auto newSeq = std::make_unique<MutableArraySequence<U>>();
    size_t idx = 0;
    for (auto it = seq_->begin(); it != seq_->end(); ++it, ++idx)
        newSeq->Append(f(*it, idx));
    return Stream<U>(std::move(newSeq));
}

template <class T>
Stream<T> Stream<T>::Where(const std::function<bool(const T&)>& pred) const {
    auto newSeq = std::make_unique<MutableArraySequence<T>>();
    for (auto it = seq_->begin(); it != seq_->end(); ++it)
        if (pred(*it)) newSeq->Append(*it);
    return Stream<T>(std::move(newSeq));
}

template <class T>
T Stream<T>::Reduce(const std::function<T(const T&, const T&)>& f, T init) const {
    T result = init;
    for (auto it = seq_->begin(); it != seq_->end(); ++it)
        result = f(result, *it);
    return result;
}

template <class T>
Stream<T> Stream<T>::GetSubsequence(size_t start, size_t end) const {
    if (start > end || end >= seq_->GetLength())
        throw std::out_of_range("Invalid subsequence indices");
    Sequence<T>* sub = seq_->GetSubsequence(start, end);
    return Stream<T>(sub);
}

template <class T>
bool Stream<T>::ContainsSubsequence(const Stream<T>& other) const {
    if (other.GetLength() == 0) return true;
    if (other.GetLength() > GetLength()) return false;
    auto first1 = seq_->begin(), last1 = seq_->end();
    auto first2 = other.seq_->begin(), last2 = other.seq_->end();
    for (auto it = first1; it != last1; ++it) {
        auto it1 = it;
        auto it2 = first2;
        while (it1 != last1 && it2 != last2 && *it1 == *it2) {
            ++it1; ++it2;
        }
        if (it2 == last2) return true;
    }
    return false;
}

template <class T>
Stream<T> Stream<T>::Merge(const Stream<T>& other) const {
    auto newSeq = std::make_unique<MutableArraySequence<T>>();
    for (auto it = seq_->begin(); it != seq_->end(); ++it)
        newSeq->Append(*it);
    for (auto it = other.seq_->begin(); it != other.seq_->end(); ++it)
        newSeq->Append(*it);
    return Stream<T>(std::move(newSeq));
}

template <class T>
std::pair<Stream<T>, Stream<T>> Stream<T>::Split(const std::function<bool(const T&)>& pred) const {
    auto trueSeq = std::make_unique<MutableArraySequence<T>>();
    auto falseSeq = std::make_unique<MutableArraySequence<T>>();
    for (auto it = seq_->begin(); it != seq_->end(); ++it) {
        if (pred(*it)) trueSeq->Append(*it);
        else falseSeq->Append(*it);
    }
    return { Stream<T>(std::move(trueSeq)), Stream<T>(std::move(falseSeq)) };
}

template <class T>
void Stream<T>::Print(std::ostream& os, const std::function<void(std::ostream&, const T&)>& printer) const {
    os << "[";
    auto it = seq_->begin();
    if (it != seq_->end()) {
        if (printer) printer(os, *it);
        else os << *it;
        ++it;
    }
    for (; it != seq_->end(); ++it) {
        os << ", ";
        if (printer) printer(os, *it);
        else os << *it;
    }
    os << "]";
}

template <class T>
T Stream<T>::operator[](size_t index) const { return seq_->Get(index); }

template <class T>
Stream<T> Stream<T>::operator+(const Stream<T>& other) const {
    return this->Merge(other);
}

template <class T>
Stream<T> Stream<T>::operator+(const T& item) const {
    auto newSeq = std::make_unique<MutableArraySequence<T>>();
    for (auto it = seq_->begin(); it != seq_->end(); ++it)
        newSeq->Append(*it);
    newSeq->Append(item);
    return Stream<T>(std::move(newSeq));
}

template <class T>
Stream<T>& Stream<T>::operator+=(const T& item) {
    seq_->Append(item);
    return *this;
}

template <class T>
Stream<T>& Stream<T>::operator+=(const Stream<T>& other) {
    for (auto it = other.seq_->begin(); it != other.seq_->end(); ++it)
        seq_->Append(*it);
    return *this;
}

template <class T>
bool Stream<T>::operator==(const Stream<T>& other) const {
    if (GetLength() != other.GetLength()) return false;
    auto it1 = seq_->begin();
    auto it2 = other.seq_->begin();
    while (it1 != seq_->end() && it2 != other.seq_->end()) {
        if (!(*it1 == *it2)) return false;
        ++it1; ++it2;
    }
    return true;
}

template <class T>
bool Stream<T>::operator!=(const Stream<T>& other) const {
    return !(*this == other);
}