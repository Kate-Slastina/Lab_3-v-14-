#pragma once
#include "ListSequence.hpp"

template<class T>
class MutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;
    ListSequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
    Sequence<T>* Instance() override { return this; }
};