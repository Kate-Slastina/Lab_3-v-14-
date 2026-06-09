#pragma once
#include "Element.hpp"
#include <QString>

class TeacherElement : public Element {
    QString name;
    double salary;
public:
    TeacherElement(const QString& n, double s);
    QString toString() const override;
    Element* clone() const override;
    Element* applyMap() const override;
    bool applyWhere() const override;
    double applyReduce() const override;
};