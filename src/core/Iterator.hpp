#pragma once
#include <cstddef>
#include <iterator>

template<typename T> class Sequence;

template<typename T>
class SequenceIterator {
private:
    const Sequence<T>* seq_;
    size_t index_;
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = const T*;
    using reference = T;

    SequenceIterator(const Sequence<T>* seq, size_t index) : seq_(seq), index_(index) {}

    reference operator*() const { return seq_->Get(index_); }
    SequenceIterator& operator++() { ++index_; return *this; }
    SequenceIterator operator++(int) { SequenceIterator tmp = *this; ++(*this); return tmp; }
    bool operator==(const SequenceIterator& other) const { return seq_ == other.seq_ && index_ == other.index_; }
    bool operator!=(const SequenceIterator& other) const { return !(*this == other); }
};