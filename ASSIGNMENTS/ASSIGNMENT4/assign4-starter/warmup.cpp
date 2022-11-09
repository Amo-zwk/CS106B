#include "datapoint.h"
#include "testing/SimpleTest.h"
#include "strlib.h"
using namespace std;

PROVIDED_TEST("Test valid use of array, new/delete") {
    DataPoint* taskList = new DataPoint[6]; // allocate

    for (int i = 0; i < 3; i++) {   // assign entries 0,1,2 (others uninitialized)
        taskList[i].priority = 1;  // set struct fields using dot notation
        taskList[i].label = "pset " + integerToString(i+1);
    }
    taskList[0].priority += 5;
    delete[] taskList; // deallocate
}
