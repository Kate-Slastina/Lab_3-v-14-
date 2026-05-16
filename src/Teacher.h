#pragma once
#include <iostream>
#include <string>

struct Teacher {
    std::string name;
    std::string subject;
    bool operator==(const Teacher& other) const {
        return name == other.name && subject == other.subject;
    }
    friend std::ostream& operator<<(std::ostream& os, const Teacher& t) {
        os << t.name << " (" << t.subject << ")";
        return os;
    }
};