#pragma once
#include <cstddef>

template <typename T>
class ICollection {
public:
    virtual ~ICollection() = default;
    virtual T Get(size_t index) const = 0;
    virtual size_t GetLength() const = 0;
    virtual bool IsEmpty() const { return GetLength() == 0; }
};