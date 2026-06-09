#include "Teacher.hpp"

Teacher::Teacher() : name(""), salary(0.0) {}
Teacher::Teacher(const std::string& n, double s) : name(n), salary(s) {}

bool Teacher::operator==(const Teacher& other) const {
    return name == other.name && std::abs(salary - other.salary) < 1e-9;
}

std::ostream& operator<<(std::ostream& os, const Teacher& t) {
    os << t.name << " (" << t.salary << ")";
    return os;
}