#include "pqheap.h"
#include "error.h"
#include "random.h"
#include "strlib.h"
#include "datapoint.h"
#include "testing/SimpleTest.h"
using namespace std;

const int INITIAL_CAPACITY = 10;
const int NONE = -1; // used as sentinel index

PQHeap::PQHeap() {
    _numAllocated = INITIAL_CAPACITY;
    _elements = new DataPoint[_numAllocated]();
    _numFilled = 0;
}

PQHeap::~PQHeap() {
    delete[] _elements;
}

DataPoint PQHeap::peek() const {
    if (isEmpty()) {
        error("PQHeap is empty!");
    }
    return _elements[0];
}

bool PQHeap::isEmpty() const {
    return _numFilled == 0;
}

int PQHeap::size() const {
    return _numFilled;
}

void PQHeap::clear() {
    _numFilled = 0;
}

void PQHeap::printDebugInfo(string msg) const {
    cout << msg << endl;
}

void PQHeap::validateInternalState() const {
    if (_numFilled > _numAllocated) error("Too many elements in not enough space!");

    /*
     * 遍历整个数组，比较当前元素的优先级与leftChild和rightChild的优先级
     * 倘若当前元素的priority数值大于其child的priority，则报错
     */
    for (int i = 0; i < size(); i++) {
        int leftChild = getLeftChildIndex(i);
        int rightChild = getRightChildIndex(i);
        if(leftChild != NONE && _elements[i].priority - _elements[leftChild].priority > 0){
            printDebugInfo("validateInternalState");
            error("PQHeap has elements out of order at index " + integerToString(leftChild));
        }
        if(rightChild != NONE && _elements[i].priority - _elements[rightChild].priority > 0){
            printDebugInfo("validateInternalState");
            error("PQHeap has elements out of order at index " + integerToString(rightChild));
        }
    }
}

int PQHeap::getParentIndex(int child) const {
    return (child - 1) / 2;
}

int PQHeap::getLeftChildIndex(int parent) const {
    return (parent * 2) + 1;
}

int PQHeap::getRightChildIndex(int parent) const {
    return (parent * 2) + 2;
}

void PQHeap::swap(int indexA, int indexB) {
    DataPoint tmp = _elements[indexA];
    _elements[indexA] = _elements[indexB];
    _elements[indexB] = tmp;
}

void PQHeap::swim(int x) {
    while(x > 0 && _elements[PQHeap::getParentIndex(x)].priority > _elements[x].priority) {
        PQHeap::swap(PQHeap::getParentIndex(x),x);
        x = PQHeap::getParentIndex(x);
    }
}

void PQHeap::sink(int i) {
    while(i < _numFilled) {
        int left = 2 * i + 1;
        int right = 2 * i + 1;
        int maxium = i;

        if(left < _numFilled && _elements[maxium].priority > _elements[left].priority) maxium = left;
        if(right < _numFilled && _elements[right].priority > _elements[left].priority) maxium = right;
        if(maxium == i) break;
        PQHeap::swap(maxium,i);
        i = maxium;
    }
}

void PQHeap::resize(int size) {
    DataPoint* copyArray = _elements;
    int beforeSize = _numAllocated;
    _numAllocated = size;
    _elements = new DataPoint[_numAllocated]();
    for(int i = 0; i < beforeSize; i++)
        _elements[i] = copyArray[i];
}

void PQHeap::enqueue(DataPoint elem) {
    if(_numFilled == _numAllocated)
        resize(_numAllocated * 2);
    _elements[_numFilled] = elem;
    swim(_numFilled);
    _numFilled++;
}

DataPoint PQHeap::dequeue() {
    auto minium = _elements[0];
    PQHeap::swap(0,_numFilled-1);
    _elements[_numFilled - 1] = {};
    _numFilled--;
    sink(0);
    return minium;
}
