#pragma once
#include <iostream>
#include <string>
#include <cmath>

struct Teacher {
    std::string name;
    double salary;
    Teacher();
    Teacher(const std::string& n, double s);
    bool operator==(const Teacher& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Teacher& t);
};