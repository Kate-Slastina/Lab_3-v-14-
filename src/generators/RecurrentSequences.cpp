#include "RecurrentSequences.hpp"
#include "MutableArraySequence.hpp"
#include <memory>

Stream<long long> Recurrent::GenerateFactorialSequence(size_t length) {
    auto seq = std::make_unique<MutableArraySequence<long long>>();
    long long val = 1;
    for (size_t i = 0; i < length; ++i) {
        if (i == 0) val = 1;
        else val *= static_cast<long long>(i);
        seq->Append(val);
    }
    return Stream<long long>(std::move(seq));
}

Stream<long long> Recurrent::GenerateFibonacciSequence(size_t length) {
    auto seq = std::make_unique<MutableArraySequence<long long>>();
    if (length == 0) return Stream<long long>(std::move(seq));
    long long a = 0, b = 1;
    seq->Append(a);
    if (length == 1) return Stream<long long>(std::move(seq));
    seq->Append(b);
    for (size_t i = 2; i < length; ++i) {
        long long next = a + b;
        seq->Append(next);
        a = b;
        b = next;
    }
    return Stream<long long>(std::move(seq));
}