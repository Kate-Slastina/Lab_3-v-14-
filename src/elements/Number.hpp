#pragma once
#include "Element.hpp"

class Number : public Element {};

class IntegerNumber : public Number {
    long long value;
public:
    IntegerNumber(long long v);
    QString toString() const override;
    Element* clone() const override;
    Element* applyMap() const override;
    bool applyWhere() const override;
    double applyReduce() const override;
};

class RealNumber : public Number {
    double value;
public:
    RealNumber(double v);
    QString toString() const override;
    Element* clone() const override;
    Element* applyMap() const override;
    bool applyWhere() const override;
    double applyReduce() const override;
};

class ComplexNumber : public Number {
    double re, im;
public:
    ComplexNumber(double r, double i);
    QString toString() const override;
    Element* clone() const override;
    Element* applyMap() const override;
    bool applyWhere() const override;
    double applyReduce() const override;
};