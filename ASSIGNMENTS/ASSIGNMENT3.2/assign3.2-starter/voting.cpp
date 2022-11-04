#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "voting.h"
#include "testing/SimpleTest.h"
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include <algorithm>

using namespace std;

/*
 * 就是计算出给出的的Vector<int>& blocks的所有值然后折半然后+1记为n,然后找出所有大于等于n的block
 * 然后计算出每个block的所有子集中,然后找少了哪个子集就不能满足大于等于n的这些子集记为m
 * 最后算出所有的block的m的取值,然后算出对应的占比然后乘百分百,就是答案
 */

class Banzhaf {
private:
    int border;
    int size;
    Vector<bool> status;
    std::unordered_map<int, std::pair<int,int>> res;
    int C(int x, int y) {
        return std::tgamma(y + 1) / std::tgamma(x + 1) / std::tgamma(y - x + 1);
    }
    void dfs(const Vector<int>& blocks, int index, int total) {
        if (index == size) {
            if (total >= border) {
                std::unordered_map<int, int> tmp,tmp2;
                for (int i = 0; i < size; i++) {
                    int now = blocks[i];
                    if (status[i]) {
                        tmp[now]++;
                        if((total < border + now))
                            tmp2[now]++;
                    }
                }
                int ans = 1;
                for (auto [x, y] : tmp)
                    ans *= C(y, res[x].first);
                for (auto [x, y] : tmp2)
                    res[x].second += ans * y;
            }
            return;
        }
        if (index > 0 && blocks[index] == blocks[index - 1] && !status[index - 1]) {
            dfs(blocks, index + 1, total);
        }else {
            status[index] = true;
            dfs(blocks, index + 1, total + blocks[index]);
            status[index] = false;
            dfs(blocks, index + 1, total);
        }
    }
public:
    Banzhaf(Vector<int> blocks)
        : size(blocks.size()), status(size, false) {
        border = std::reduce(blocks.begin(), blocks.end()) / 2 + 1;
        std::sort(blocks.begin(), blocks.end());
        for (auto x : blocks)
            res[x].first++;
        dfs(blocks, 0, 0);
    }
    Vector<int> get(const Vector<int>& blocks) {
        int total = 0;
        Vector<int> ans(size);
        for (auto [x, y] : res)
            total += y.second;
        for (int i = 0; i < size; i++)
            ans[i] = (res[blocks[i]].second/ res[blocks[i]].first * 100) / total;
        return ans;
    }
};
Vector<int> computePowerIndexes(Vector<int>& blocks) {
    return Banzhaf(blocks).get(blocks);
}

/* * * * * * Test Cases * * * * * */

// 测试listSubsetsOfRec函数

PROVIDED_TEST("测试listSubsetsOfRec函数1") {
    Vector<int> blocks = {50, 49, 1};
    blocks = computePowerIndexes(blocks);
    Vector<int> expected = {60, 20, 20};
    EXPECT_EQUAL(blocks, expected);
}


PROVIDED_TEST("测试listSubsetsOfRec函数2") {
    Vector<int> blocks = {1, 1, 3, 7, 9, 9};
    Vector<int> expected = {0, 0, 0, 33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("测试listSubsetsOfRec函数3") {
    Vector<int> blocks = {55, 38, 29};
    Vector<int> expected = {33, 33, 33};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("测试listSubsetsOfRec函数4") {
    Vector<int> blocks = {55, 38, 16};
    Vector<int> expected = {100, 0, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}
PROVIDED_TEST("测试listSubsetsOfRec函数5") {
    // Estonia is one of those 4s!!
    Vector<int> blocks = {29,29,29,29,27,27,14,13,12,12,12,12,12,10,10,10,7,7,7,7,7,4,4,4,4,4,3};
    Vector<int> expected = {8, 8, 8, 8, 7, 7, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}
PROVIDED_TEST("Time power index operation") {
    Vector<int> blocks;
    for (int i = 0; i < 27; i++) {
        blocks.add(randomInteger(1, 10));
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}


