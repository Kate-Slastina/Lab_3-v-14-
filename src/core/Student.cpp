#include "Student.hpp"

Student::Student() : name(""), grade(0.0) {}
Student::Student(const std::string& n, double g) : name(n), grade(g) {}

bool Student::operator==(const Student& other) const {
    return name == other.name && std::abs(grade - other.grade) < 1e-9;
}

std::ostream& operator<<(std::ostream& os, const Student& s) {
    os << s.name << " (" << s.grade << ")";
    return os;
}