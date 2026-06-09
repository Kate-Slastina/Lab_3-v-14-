#include "StudentElement.hpp"
#include <algorithm>

StudentElement::StudentElement(const QString& n, double g) : name(n), grade(g) {}
QString StudentElement::toString() const { return name + " (" + QString::number(grade) + ")"; }
Element* StudentElement::clone() const { return new StudentElement(name, grade); }
Element* StudentElement::applyMap() const { return new StudentElement(name, std::max(grade - 0.5, 2.0)); }
bool StudentElement::applyWhere() const { return grade >= 3.0; }
double StudentElement::applyReduce() const { return grade; }