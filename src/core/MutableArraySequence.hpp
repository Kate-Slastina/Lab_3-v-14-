#pragma once
#include "ArraySequence.hpp"

template<class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;
    ArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
    Sequence<T>* Instance() override { return this; }
};