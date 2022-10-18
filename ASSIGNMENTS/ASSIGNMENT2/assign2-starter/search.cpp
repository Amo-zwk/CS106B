#include "search.h"
#include "error.h"
#include "filelib.h"
#include "map.h"
#include "set.h"
#include "simpio.h"
#include "strlib.h"
#include "testing/SimpleTest.h"
#include "vector.h"
#include <fstream>
#include <iostream>

using namespace std;

//cleanToken函数
string cleanToken(string s)
{
    int start = 0;
    //从第一个标点开始到第一个非标点结束
    while(start < s.size()&&ispunct(s[start])) {
        start++;
    }
    if(start > 0) {
        s.erase(0,start); //左闭右开
    }
    int end = s.size() - 1;
    while(end > 0 && ispunct(s[end])) {
        end--;
    }
    if(end < s.size() - 1) {
        s.erase(end + 1,s.size());
    }
    bool flag = false ;
    for(int i = 0; i < s.size(); i ++) {
        if(isalpha(s[i])) {
            flag = true;
            break;
        }
    }
    if(!flag) { //一个字母也没有 返回空
        return "";
    }
    return toLowerCase(s);
}

//gatherTokens函数
Set<string> gatherTokens(string text)
{
    Set<string> tokens;
    Vector<string> str = stringSplit(text, " ");
    for(auto s : str) {
        //处理每个子字符串
        string substr = cleanToken(s);
        //避免把空字符串加到Set里面
        if(substr.size()) {
            tokens.add(substr); //Set本身就不会加入重复元素,所以无需担心重复的单词
        }
    }
    return tokens;
}

//buildIndex函数
int buildIndex(string dbfile, Map<string, Set<string>> &index) {
    //读取dbfile
    ifstream in;
    if (!openFile(in, dbfile))
        error("Cannot open file named " + dbfile);
    Vector<string> lines;
    readEntireFile(in, lines);

    //构建反向索引
    int pages = 0;
    for (int url = 0; url < lines.size(); url += 2) {
        // 取出网页中的 tokens
        Set<string> tokens = gatherTokens(lines[url + 1]);
        //对每一个token绑定页面url
        for (const string &token : tokens) {
            index[token].add(lines[url]);
        }
        pages++;
    }

    return pages;
}

//findQueryMatches函数
Set<string> findQueryMatches(Map<string, Set<string>> &index, string query) {
    Set<string> result;
    // 拆分关键词
    Vector<string> tokens = stringSplit(query, " ");
    // 遍历关键词并整合结果
    for (const string &token : tokens) {
        // 处理交集
        if (startsWith(token, "+")) {
            result.intersect(index[cleanToken(token)]);
        }
        // 处理差集
        else if (startsWith(token, "-")) {
            result.difference(index[cleanToken(token)]);
        }
        // 处理并集
        else {
            result.unionWith(index[cleanToken(token)]);
        }
    }

    return result;
}

//searchEngine函数
void searchEngine(string dbfile)
{
    // 根据 dbfile 构建反向索引
    Map<string, Set<string>> index;
    int pages = buildIndex(dbfile, index);
    // 打印信息
    cout << "Stand by while building index..." << endl;
    cout << "Indexed " << pages << " pages containing " << index.size() << " unique terms" << endl;
    // 循环提示用户输入查询关键词
    while (true) {
        string terms = getLine("Enter query sentence (RETURN/ENTER to quit): ");
        // 当用户输入空字符串时，提示查询完成，程序结束。
        if (terms.empty()) {
            break;
        }
        // 对于每一个查询关键词，找出所有匹配页面并打印出 URLs
        Set<string> matches = findQueryMatches(index, terms);
        cout << "Found " << matches.size() << " matching pages" << endl;
        cout << matches << endl;
        cout << endl;
    }
}

/* * * * * * Test Cases * * * * * */

//cleanToken函数官方测试用例1
PROVIDED_TEST("cleanToken on strings with no punctuation at beginning or end") {
    EXPECT_EQUAL(cleanToken("hello"), "hello");
    EXPECT_EQUAL(cleanToken("WORLD"), "world");
    EXPECT_EQUAL(cleanToken("CS*106B"), "cs*106b");
}

//cleanToken函数官方测试用例2
PROVIDED_TEST("cleanToken on strings with some punctuation at beginning and end") {
    EXPECT_EQUAL(cleanToken("/hello/"), "hello");
    EXPECT_EQUAL(cleanToken("~woRLD!"), "world");
}

//cleanToken函数官方测试用例3
PROVIDED_TEST("cleanToken on non-word strings (no letters)"){
    EXPECT_EQUAL(cleanToken("106"), "");
    EXPECT_EQUAL(cleanToken("~!106!!!"), "");
}

//gatherTokens函数官方测试用例1
PROVIDED_TEST("gatherTokens from simple string") {
    Set<string> expected = {"go", "gophers"};
    EXPECT_EQUAL(gatherTokens("go go go gophers"), expected);
}

//gatherTokens函数官方测试用例2
PROVIDED_TEST("gatherTokens correctly cleans tokens") {
    Set<string> expected = {"i", "love", "cs*106b"};
    EXPECT_EQUAL(gatherTokens("I _love_ CS*106B!"), expected);
}

//gatherTokens函数官方测试用例3
PROVIDED_TEST("gatherTokens from seuss, 5 unique words, mixed case, punctuation") {
    Set<string> tokens = gatherTokens("One Fish Two Fish *Red* fish Blue fish ** 10 RED Fish?");
    EXPECT_EQUAL(tokens.size(), 5);
    EXPECT(tokens.contains("fish"));
    EXPECT(!tokens.contains("Fish"));
}

//buildIndex函数官方测试用例1
PROVIDED_TEST("buildIndex from tiny.txt, 4 pages, 11 unique tokens") {
    Map<string, Set<string>> index;
    int nPages = buildIndex("res/tiny.txt", index);
    EXPECT_EQUAL(nPages, 4);
    EXPECT_EQUAL(index.size(), 11);
    EXPECT(index.containsKey("fish"));
}

//findQueryMatches函数官方测试用例1
PROVIDED_TEST("findQueryMatches from tiny.txt, single word query") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRed = findQueryMatches(index, "red");
    EXPECT_EQUAL(matchesRed.size(), 2);
    EXPECT(matchesRed.contains("www.dr.seuss.net"));
    Set<string> matchesHippo = findQueryMatches(index, "hippo");
    EXPECT(matchesHippo.isEmpty());
}

//findQueryMatches函数官方测试用例2
PROVIDED_TEST("findQueryMatches from tiny.txt, compound queries") {
    Map<string, Set<string>> index;
    buildIndex("res/tiny.txt", index);
    Set<string> matchesRedOrFish = findQueryMatches(index, "red fish");
    EXPECT_EQUAL(matchesRedOrFish.size(), 4);
    Set<string> matchesRedAndFish = findQueryMatches(index, "red +fish");
    EXPECT_EQUAL(matchesRedAndFish.size(), 1);
    Set<string> matchesRedWithoutFish = findQueryMatches(index, "red -fish");
    EXPECT_EQUAL(matchesRedWithoutFish.size(), 1);
}
