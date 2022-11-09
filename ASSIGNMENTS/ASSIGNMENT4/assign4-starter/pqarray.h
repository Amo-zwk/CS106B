#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

class PQArray {

public:
    void resize(int size);
    PQArray();
    ~PQArray();
    void enqueue(DataPoint element);
    DataPoint dequeue();
    DataPoint peek() const;
    bool isEmpty() const;
    int size() const;
    void clear();
    void printDebugInfo(std::string msg) const;
    void validateInternalState() const;

private:
    DataPoint* _elements;   // dynamic array
    int _numAllocated;      // number of slots allocated in array
    int _numFilled;         // number of slots filled in array
    void swap(int indexA, int indexB);
    DISALLOW_COPYING_OF(PQArray);
};
