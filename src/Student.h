#pragma once
#include <iostream>
#include <string>

struct Student {
    std::string name;
    int id;
    bool operator==(const Student& other) const { return id == other.id; }
    friend std::ostream& operator<<(std::ostream& os, const Student& s) {
        os << s.name << "(" << s.id << ")";
        return os;
    }
};