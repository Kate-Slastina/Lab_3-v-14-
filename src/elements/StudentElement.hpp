#pragma once
#include "Element.hpp"
#include <QString>

class StudentElement : public Element {
    QString name;
    double grade;
public:
    StudentElement(const QString& n, double g);
    QString toString() const override;
    Element* clone() const override;
    Element* applyMap() const override;
    bool applyWhere() const override;
    double applyReduce() const override;
};