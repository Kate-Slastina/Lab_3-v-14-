#pragma once
#include "Sequence.hpp"
#include <functional>
#include <utility>
#include <stdexcept>

template <class T>
class Stream {
private:
    Sequence<T>* seq_;   

public:
    Stream() : seq_(nullptr) {}
    explicit Stream(Sequence<T>* seq) : seq_(seq) {}
    ~Stream() { delete seq_; }

    Stream(const Stream&) = delete;
    Stream(Stream&& other) noexcept : seq_(other.seq_) { other.seq_ = nullptr; }
    Stream& operator=(const Stream&) = delete;
    Stream& operator=(Stream&& other) noexcept {
        if (this != &other) { delete seq_; seq_ = other.seq_; other.seq_ = nullptr; }
        return *this;
    }

    T GetFirst() const { return seq_->GetFirst(); }
    T GetLast() const { return seq_->GetLast(); }
    T Get(size_t index) const { return seq_->Get(index); }
    size_t GetLength() const { return seq_->GetLength(); }

    Stream<T>* Map(const std::function<T(const T&)>& f) const;
    Stream<T>* Where(const std::function<bool(const T&)>& pred) const;
    T Reduce(const std::function<T(const T&, const T&)>& f, T init) const;
    Stream<T>* GetSubsequence(size_t start, size_t end) const;
    bool ContainsSubsequence(const Stream<T>& other) const;
    Stream<T>* Merge(const Stream<T>& other) const;
    std::pair<Stream<T>*, Stream<T>*> Split(const std::function<bool(const T&)>& pred) const;

    void Print(std::ostream& os, const std::function<void(std::ostream&, const T&)>& printer = nullptr) const;
};

#include "Stream.tpp"