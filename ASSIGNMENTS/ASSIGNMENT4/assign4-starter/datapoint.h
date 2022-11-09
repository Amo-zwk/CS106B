#pragma once

#include <string>
#include <ostream>
#include <istream>

struct DataPoint {
    std::string label;
    double priority;
};

bool operator== (const DataPoint& lhs, const DataPoint& rhs);
bool operator!= (const DataPoint& lhs, const DataPoint& rhs);

std::ostream& operator<< (std::ostream& out, const DataPoint& pt);

std::istream& operator>> (std::istream& in, DataPoint& result);

bool operator<  (const DataPoint& lhs, const DataPoint& rhs) = delete;
bool operator<= (const DataPoint& lhs, const DataPoint& rhs) = delete;
bool operator>= (const DataPoint& lhs, const DataPoint& rhs) = delete;
bool operator>  (const DataPoint& lhs, const DataPoint& rhs) = delete;
