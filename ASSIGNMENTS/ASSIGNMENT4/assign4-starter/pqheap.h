#pragma once
#include "testing/MemoryUtils.h"
#include "datapoint.h"

class PQHeap {

public:
    PQHeap();
    ~PQHeap();
    void enqueue(DataPoint element);
    DataPoint dequeue();
    DataPoint peek() const;
    bool isEmpty() const;
    int size() const;
    void clear();
    void swim(int x);
    void sink(int x);
    void printDebugInfo(std::string msg) const;
    void validateInternalState() const;

private:
    void resize(int size);
    void swap(int indexA, int indexB);
    DataPoint* _elements;
    int _numFilled;
    int _numAllocated;
    int getParentIndex(int child) const;
    int getLeftChildIndex(int parent) const;
    int getRightChildIndex(int parent) const;
    DISALLOW_COPYING_OF(PQHeap);
};
