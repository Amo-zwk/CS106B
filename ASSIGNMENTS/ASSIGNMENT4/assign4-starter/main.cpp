#include <iostream>
#include "console.h"
#include "pqclient.h"
#include "pqarray.h"
#include "pqheap.h"
#include "testing/SimpleTest.h"
using namespace std;

int main()
{
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }
    cout << "All done, exiting" << endl;
    return 0;
}

void confirmFunctionPrototypes() {
    {
        PQArray pq;
        pq.enqueue({ "", 1 });
        DataPoint n = pq.peek();
        n = pq.dequeue();
        int size = pq.size();
        pq.isEmpty();
        pq.clear();
    }
    Vector<DataPoint> v;
    pqSort(v);
    v = topK(cin, 1);
    {
        PQHeap pq;
        pq.enqueue({ "", 1 });
        DataPoint n = pq.peek();
        n = pq.dequeue();
        int size = pq.size();
        pq.isEmpty();
        pq.clear();
    }
}
