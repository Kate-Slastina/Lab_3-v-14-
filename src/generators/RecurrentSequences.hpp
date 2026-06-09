#pragma once
#include "Stream.hpp"

namespace Recurrent {
    Stream<long long> GenerateFactorialSequence(size_t length);
    Stream<long long> GenerateFibonacciSequence(size_t length);
}