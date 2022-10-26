#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "recursion.h"
#include "testing/SimpleTest.h"

using namespace std;

//operatorsFrom函数:找出所有括号
string operatorsFrom(string str) {
    string operators = "[]{}()"; //答案初始化为空
    string res = "";
    if(str.empty()) {
        return "";
    }
    else if(operators.find(str[0]) != string::npos) {
        res += str[0];
    }
    return  res + operatorsFrom(str.substr(1));
}

//operatorsAreMatched函数:括号匹配
bool operatorsAreMatched(string ops) {
    if(ops.empty()) {
        return true;
    }else if(stringContains(ops,"()")) {
        int start = stringIndexOf(ops,"()");
        ops.erase(start,2);
    }else if(stringContains(ops,"[]")) {
        int start = stringIndexOf(ops,"[]");
        ops.erase(start,2);
    }else if(stringContains(ops,"{}")) {
        int start = stringIndexOf(ops,"{}");
        ops.erase(start,2);
    }
    else {
        return false;
    }
    return operatorsAreMatched(ops);
}

//isBalanced函数:判断是否匹配
bool isBalanced(string str) {
    string ops = operatorsFrom(str);
    return operatorsAreMatched(ops);
}


/* * * * * * Test Cases * * * * * */

//operatorsAreMatched测试用例1
PROVIDED_TEST("operatorsAreMatched测试用例1") {
    EXPECT(operatorsAreMatched("[]"));
}
//operatorsAreMatched测试用例2
PROVIDED_TEST("operatorsAreMatched测试用例2") {
    EXPECT(operatorsAreMatched("{([](){})}"));
}
//operatorsAreMatched测试用例3
PROVIDED_TEST("operatorsAreMatched测试用例3") {
    EXPECT(operatorsAreMatched("[(){}]"));
}
//operatorsAreMatched测试用例4
PROVIDED_TEST("operatorsAreMatched测试用例4") {
    EXPECT(operatorsAreMatched("[{}]"));
}
//operatorsAreMatched测试用例5
PROVIDED_TEST("operatorsAreMatched测试用例5") {
    EXPECT(operatorsAreMatched("[]"));
}
//operatorsAreMatched测试用例6
PROVIDED_TEST("operatorsAreMatched测试用例6") {
    EXPECT(operatorsAreMatched(""));
}
//operatorsAreMatched测试用例7
PROVIDED_TEST("operatorsAreMatched测试用例7") {
    EXPECT(!operatorsAreMatched("([)]"));
}
//operatorsAreMatched测试用例8
PROVIDED_TEST("operatorsAreMatched测试用例8") {
    EXPECT(operatorsAreMatched("({((()))})"));
}
//operatorsAreMatched测试用例9
PROVIDED_TEST("operatorsAreMatched测试用例9") {
    EXPECT(!operatorsAreMatched("({)))(({"));
}
//operatorsFrom测试用例1
PROVIDED_TEST("operatorsFrom测试用例1") {
    EXPECT_EQUAL(operatorsFrom("vec[3]"), "[]");
}

//operatorsFrom测试用例2
PROVIDED_TEST("operatorsFrom测试用例2") {
    EXPECT_EQUAL(operatorsFrom("(([a])"), "(([])");
}

//operatorsFrom测试用例3
PROVIDED_TEST("operatorsFrom测试用例3") {
    EXPECT_EQUAL(operatorsFrom("3)("), ")(");
}

//operatorsFrom测试用例4
PROVIDED_TEST("operatorsFrom测试用例4") {
    EXPECT_EQUAL(operatorsFrom("{(x}y)"), "{(})");
}

//operatorsFrom测试用例5
PROVIDED_TEST("operatorsFrom测试用例5") {
    EXPECT_EQUAL(operatorsFrom(" "), "");
}

//operatorsFrom测试用例6
PROVIDED_TEST("operatorsFrom测试用例6") {
    EXPECT_EQUAL(operatorsFrom(""), "");
}

//operatorsFrom测试用例7
PROVIDED_TEST("operatorsFrom测试用例7") {
    EXPECT_EQUAL(operatorsFrom("(){([])(())}7897979asdaqweqexvxrthhhh"), "(){([])(())}");
}

//PROVIDED_TEST("isBalanced on example from writeup") {
//    string example ="int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
//    EXPECT(isBalanced(example));
//}

//PROVIDED_TEST("isBalanced on non-balanced examples from writeup") {
//    EXPECT(!isBalanced("( ( [ a ] )"));
//    EXPECT(!isBalanced("3 ) ("));
//    EXPECT(!isBalanced("{ ( x } y )"));
//}
