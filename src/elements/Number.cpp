#include "Number.hpp"
#include <QString>
#include <cmath>

IntegerNumber::IntegerNumber(long long v) : value(v) {}
QString IntegerNumber::toString() const { return QString::number(value); }
Element* IntegerNumber::clone() const { return new IntegerNumber(value); }
Element* IntegerNumber::applyMap() const { return new IntegerNumber(value * 2); }
bool IntegerNumber::applyWhere() const { return value > 10; }
double IntegerNumber::applyReduce() const { return static_cast<double>(value); }

RealNumber::RealNumber(double v) : value(v) {}
QString RealNumber::toString() const { return QString::number(value); }
Element* RealNumber::clone() const { return new RealNumber(value); }
Element* RealNumber::applyMap() const { return new RealNumber(value * 2); }
bool RealNumber::applyWhere() const { return value > 10.0; }
double RealNumber::applyReduce() const { return value; }

ComplexNumber::ComplexNumber(double r, double i) : re(r), im(i) {}
QString ComplexNumber::toString() const { return QString("%1%2%3i").arg(re).arg(im>=0?"+":"").arg(im); }
Element* ComplexNumber::clone() const { return new ComplexNumber(re, im); }
Element* ComplexNumber::applyMap() const { return new ComplexNumber(re*2, im*2); }
bool ComplexNumber::applyWhere() const { return false; }
double ComplexNumber::applyReduce() const { return re; }