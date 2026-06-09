#include "Complex.hpp"

Complex::Complex(double r, double i) : re(r), im(i) {}

bool Complex::operator==(const Complex& other) const {
    const double eps = 1e-9;
    return std::abs(re - other.re) < eps && std::abs(im - other.im) < eps;
}

std::ostream& operator<<(std::ostream& os, const Complex& c) {
    os << c.re;
    if (c.im >= 0) os << "+";
    os << c.im << "i";
    return os;
}