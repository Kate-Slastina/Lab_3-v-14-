#pragma once
#include "Stream.hpp"

template <typename T>
size_t CountInversions(const Stream<T>& stream) {
    size_t total = 0;
    auto it_outer = stream.begin();
    while (it_outer != stream.end()) {
        const T& val = *it_outer;
        auto it_inner = stream.begin();
        while (it_inner != it_outer) {
            if (*it_inner > val) ++total;
            ++it_inner;
        }
        ++it_outer;
    }
    return total;
}