#pragma once
#include <iostream>
#include <string>
#include <cmath>

struct Student {
    std::string name;
    double grade;
    Student();
    Student(const std::string& n, double g);
    bool operator==(const Student& other) const;
    friend std::ostream& operator<<(std::ostream& os, const Student& s);
};