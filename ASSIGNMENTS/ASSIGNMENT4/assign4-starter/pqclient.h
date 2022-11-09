#pragma once

#include "datapoint.h"
#include "vector.h"
#include <istream>

void pqSort(Vector<DataPoint>& v);
Vector<DataPoint> topK(std::istream& stream, int k);
