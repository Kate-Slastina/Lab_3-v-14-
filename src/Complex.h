
#pragma once
#include <iostream>

struct Complex {
    double re, im;
    Complex(double r = 0, double i = 0) : re(r), im(i) {}
    bool operator==(const Complex& other) const {
        return re == other.re && im == other.im;
    }
    friend std::ostream& operator<<(std::ostream& os, const Complex& c) {
        os << c.re;
        if (c.im >= 0) os << "+";
        os << c.im << "i";
        return os;
    }
};