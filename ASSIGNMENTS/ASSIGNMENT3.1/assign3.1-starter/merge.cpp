#include <iostream>    // for cout, endl
#include "queue.h"
#include "testing/SimpleTest.h"
using namespace std;

//hlper函数
bool helper(Queue<int> a) {
    while(!a.isEmpty()) {
        if(a.size() == 1) {
            return true;
        }
        int n = a.dequeue(), m = a.peek();
        if(n > m) {
            return false;
        }
    }
    return true;
}

//binaryMerge函数
Queue<int> binaryMerge(Queue<int> a, Queue<int> b) {
    if(a.isEmpty() && b.isEmpty()) {
        return a;
    }
    Queue<int> result;
    if(!(helper(a)) or (!helper(b))) {
        error("队列a或者队列b不符合要求");
    }
    while((!a.isEmpty()) && (!b.isEmpty())) {
        int n = a.peek(), m = b.peek();
        if(n < m) {
            result.enqueue(a.dequeue());
        }else {
            result.enqueue(b.dequeue());
        }
    }
    while(!a.isEmpty()) {
         result.enqueue(a.dequeue());
    }
    while(!b.isEmpty()) {
         result.enqueue(b.dequeue());
    }
    return result;
}

//naiveMultiMerge函数
Queue<int> naiveMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;

    for (Queue<int>& q : all) {
        result = binaryMerge(q, result);
    }
    return result;
}

//recMultiMerge函数
Queue<int> recMultiMerge(Vector<Queue<int>>& all) {
    Queue<int> result;
    if(all.size() == 1) {
        result = binaryMerge(all.get(0),result);
        return result;
    }

    Vector<Queue<int>> left = all.subList(0,0+all.size()/2);
    Vector<Queue<int>> right = all.subList(all.size()/2,all.size() - (all.size()/2));
    return binaryMerge((recMultiMerge(left)),(recMultiMerge(right)));
}

/* * * * * * Test Cases * * * * * */

Queue<int> createSequence(int size);
void distribute(Queue<int> input, Vector<Queue<int>>& all);

//recMultiMerge函数官方测试用例1
PROVIDED_TEST("recMultiMerge函数官方测试用例1") {
    Vector<Queue<int>> all = {{1,3,4,4,5},{1,2,5,8,8,9}};
    Queue<int> expected = {1,1,2,3,4,4,5,5,8,8,9};
    EXPECT_EQUAL(recMultiMerge(all), expected);
}

//binaryMerge函数官方测试用例1
PROVIDED_TEST("binaryMerge函数官方测试用例1") {
    Queue<int> a = {2, 4, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 3, 3, 4, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
//binaryMerge函数官方测试用例2
PROVIDED_TEST("binaryMerge函数官方测试用例2") {
    Queue<int> a = {1, 1, 1};
    Queue<int> b = {1, 2, 3};
    Queue<int> expected = {1, 1, 1, 1, 2, 3};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
//binaryMerge函数官方测试用例3
PROVIDED_TEST("binaryMerge函数官方测试用例3") {
    Queue<int> a = {2, 2, 5};
    Queue<int> b = {1, 3, 3};
    Queue<int> expected = {1, 2, 2, 3, 3, 5};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
//binaryMerge函数官方测试用例4
PROVIDED_TEST("binaryMerge函数官方测试用例4") {
    Queue<int> a = {2, 2, 2};
    Queue<int> b = {1, 1, 1};
    Queue<int> expected = {1, 1, 1, 2, 2, 2};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
//binaryMerge函数官方测试用例5
PROVIDED_TEST("binaryMerge函数官方测试用例5") {
    Queue<int> a = {1, 4, 5};
    Queue<int> b = {2, 5, 8};
    Queue<int> expected = {1, 2, 4, 5, 5, 8};
    EXPECT_EQUAL(binaryMerge(a, b), expected);
    EXPECT_EQUAL(binaryMerge(b, a), expected);
}
//binaryMerge函数官方测试用例6
PROVIDED_TEST("binaryMerge函数官方测试用例6") {
    Queue<int> a = {8, 5, 0};
    Queue<int> b = {3, 1};
    Queue<int> expected = {1, 1, 2, 2, 3};
    EXPECT_ERROR(binaryMerge(a, b));
    EXPECT_ERROR(binaryMerge(b, a));
}
//naiveMultiMerge函数官方测试用例1
PROVIDED_TEST("naiveMultiMerge函数官方测试用例1") {
    Vector<Queue<int>> all = {{3, 6, 9, 9, 100},
                             {1, 5, 9, 9, 12},
                             {5},
                             {},
                             {-5, -5},
                             {3402}
                            };
    Queue<int> expected = {-5, -5, 1, 3, 5, 5, 6, 9, 9, 9, 9, 12, 100, 3402};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}

//naiveMultiMerge函数官方测试用例2
PROVIDED_TEST("naiveMultiMerge函数官方测试用例2") {
    Vector<Queue<int>> all = {{},
                             {},
                             {},
                             {},
                             {},
                             {}
                            };
    Queue<int> expected = {};
    EXPECT_EQUAL(naiveMultiMerge(all), expected);
}



//recMultiMerge函数官方测试用例1
//PROVIDED_TEST("recMultiMerge函数官方测试用例1") {
//    int n = 20;
//    Queue<int> input = createSequence(n);
//    Vector<Queue<int>> all(n);
//    distribute(input, all);
//    EXPECT_EQUAL(recMultiMerge(all), naiveMultiMerge(all));
//}

//PROVIDED_TEST("Time binaryMerge operation") {
//    int n = 1000000;
//    Queue<int> a = createSequence(n);
//    Queue<int> b = createSequence(n);
//    TIME_OPERATION(a.size() + b.size(), binaryMerge(a, b));
//}

//PROVIDED_TEST("Time naiveMultiMerge operation") {
//    int n = 11000;
//    int k = n/10;
//    Queue<int> input = createSequence(n);
//    Vector<Queue<int>> all(k);
//    distribute(input, all);
//    TIME_OPERATION(input.size(), naiveMultiMerge(all));
//}
//PROVIDED_TEST("Time recMultiMerge operation") {
//    int n = 90000;
//    int k = n/10;
//    Queue<int> input = createSequence(n);
//    Vector<Queue<int>> all(k);
//    distribute(input, all);
//    TIME_OPERATION(input.size(), recMultiMerge(all));
//}


/* Test helper to fill queue with sorted sequence */
Queue<int> createSequence(int size) {
    Queue<int> q;
    for (int i = 0; i < size; i++) {
        q.enqueue(i);
    }
    return q;
}

/* Test helper to distribute elements of sorted sequence across k sequences,
   k is size of Vector */
void distribute(Queue<int> input, Vector<Queue<int>>& all) {
    while (!input.isEmpty()) {
        all[randomInteger(0, all.size()-1)].enqueue(input.dequeue());
    }
}
