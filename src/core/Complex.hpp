#pragma once
#include <iostream>
#include <cmath>

struct Complex {
    double re, im;
    Complex(double r = 0, double i = 0);
    bool operator==(const Complex& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Complex& c);
};