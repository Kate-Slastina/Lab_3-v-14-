#pragma once
#include <QString>

class Element {
public:
    virtual ~Element() = default;
    virtual QString toString() const = 0;
    virtual Element* clone() const = 0;
    virtual Element* applyMap() const = 0;
    virtual bool applyWhere() const = 0;
    virtual double applyReduce() const = 0;
};