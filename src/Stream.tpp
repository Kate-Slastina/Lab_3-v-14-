
#pragma once
#include "Stream.hpp"
#include "core/Array/MutableArraySequence.hpp"

template<class T>
Stream<T>* Stream<T>::Map(const std::function<T(const T&)>& f) const {
    auto* newSeq = new MutableArraySequence<T>();
    size_t len = seq_->GetLength();
    for (size_t i = 0; i < len; ++i) {
        newSeq->Append(f(seq_->Get(i)));
    }
    return new Stream<T>(newSeq);
}

template<class T>
Stream<T>* Stream<T>::Where(const std::function<bool(const T&)>& pred) const {
    auto* newSeq = new MutableArraySequence<T>();
    size_t len = seq_->GetLength();
    for (size_t i = 0; i < len; ++i) {
        T val = seq_->Get(i);
        if (pred(val)) newSeq->Append(val);
    }
    return new Stream<T>(newSeq);
}

template<class T>
T Stream<T>::Reduce(const std::function<T(const T&, const T&)>& f, T init) const {
    T result = init;
    size_t len = seq_->GetLength();
    for (size_t i = 0; i < len; ++i) {
        result = f(result, seq_->Get(i));
    }
    return result;
}

template<class T>
Stream<T>* Stream<T>::GetSubsequence(size_t start, size_t end) const {
    if (start > end || end >= seq_->GetLength())
        throw std::out_of_range("Invalid subsequence indices");
    Sequence<T>* sub = seq_->GetSubsequence(start, end);
    return new Stream<T>(sub);
}

template<class T>
bool Stream<T>::ContainsSubsequence(const Stream<T>& other) const {
    size_t n = seq_->GetLength();
    size_t m = other.GetLength();
    if (m == 0) return true;
    if (m > n) return false;
    for (size_t i = 0; i <= n - m; ++i) {
        bool match = true;
        for (size_t j = 0; j < m; ++j) {
            if (!(seq_->Get(i + j) == other.Get(j))) {
                match = false;
                break;
            }
        }
        if (match) return true;
    }
    return false;
}

template<class T>
Stream<T>* Stream<T>::Merge(const Stream<T>& other) const {
    auto* newSeq = new MutableArraySequence<T>();
    size_t len1 = seq_->GetLength();
    for (size_t i = 0; i < len1; ++i) newSeq->Append(seq_->Get(i));
    size_t len2 = other.GetLength();
    for (size_t i = 0; i < len2; ++i) newSeq->Append(other.Get(i));
    return new Stream<T>(newSeq);
}

template<class T>
std::pair<Stream<T>*, Stream<T>*> Stream<T>::Split(const std::function<bool(const T&)>& pred) const {
    auto* trueSeq = new MutableArraySequence<T>();
    auto* falseSeq = new MutableArraySequence<T>();
    size_t len = seq_->GetLength();
    for (size_t i = 0; i < len; ++i) {
        T val = seq_->Get(i);
        if (pred(val)) trueSeq->Append(val);
        else falseSeq->Append(val);
    }
    return { new Stream<T>(trueSeq), new Stream<T>(falseSeq) };
}

template<class T>
void Stream<T>::Print(std::ostream& os, const std::function<void(std::ostream&, const T&)>& printer) const {
    os << "[";
    size_t len = seq_->GetLength();
    for (size_t i = 0; i < len; ++i) {
        if (printer) printer(os, seq_->Get(i));
        else os << seq_->Get(i);
        if (i + 1 < len) os << ", ";
    }
    os << "]";
}