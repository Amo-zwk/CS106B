/*
 * TODO: remove and replace this file header comment
 * This is a .cpp file you will edit and turn in.
 * Remove starter comments and add your own
 * comments on each function and on complex code sections.
 */
#include "testing/SimpleTest.h"
#include "strlib.h"
#include "filelib.h"
#include "simpio.h"
#include <fstream>
#include <cctype>
#include <string>
#include "vector.h"
using namespace std;

/* This function is intended to return a string which
 * contains only the letter characters from the original
 * (all non-letter characters are removed)
 *
 * WARNING: The provided code is buggy!
 *
 * Use unit tests to identify which inputs to this function
 * are incorrectly handled. Then, remove this comment and
 * replace it with a description of the bug you fixed.
 */
string removeNonLetters(string s) {
    string str = "";
    for (int i = 0; i < s.length(); i++) {
        if (isalpha(s[i])) {
            str += s[i];
        }
    }
    return str;
}


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */

string soundex(string s) {


    string ans = "", str = "";
    vector<char> res,result;

    s = removeNonLetters(s);
    s = toUpperCase(s);

    for(int i = 0; i < s.size() ; i ++) {
        if(s[i] == 'A' or s[i] == 'E' or s[i] == 'I' or s[i] == 'O' or s[i] == 'U' or s[i] == 'H' or s[i] == 'W' or s[i] == 'Y') {
            ans += '0';
        }
        else if(s[i] == 'C' or s[i] == 'G' or s[i] == 'J' or s[i] == 'K' or s[i] == 'Q' or s[i] == 'S' or s[i] == 'X' or s[i] == 'Z') {
            ans += '2';
        }
        else if(s[i] == 'B' or s[i] == 'F' or s[i] == 'P' or s[i] == 'V') {
            ans += '1';
        }
        else if(s[i] == 'D' or s[i] == 'T') {
            ans += '3';
        }
        else if(s[i] == 'N' or s[i] == 'M') {
            ans += '5';
        }
        else if(s[i] == 'L') {
            ans += '4';
        }
        else if(s[i] == 'R'){
            ans += '6';
        }
    }

    res.push_back(ans[0]);

    for(int i = 0; i < ans.size()-1; i++) {
        if(ans[i] == ans[i+1]) {
            continue;
        }
        else {
            res.push_back(ans[i+1]);
        }
    }

    result.push_back(s[0]);

    for(int i = 1; i < res.size(); i ++) {
        if(result.size() < 4) {
        if(res[i] != '0') {
            result.push_back(res[i]);
        }
        }
    }

    int len = result.size();

    for(int i = 0; i < 4 - len; i++) {
            result.push_back('0');
    }

    for(int i = 0; i < result.size(); i ++) {
        str.push_back(result[i]);
    }

    return str;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void soundexSearch(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> databaseNames;

    if (openFile(in, filepath)) {
        readEntireFile(in, databaseNames);
    }
    cout << "Read file " << filepath << ", "
         << databaseNames.size() << " names found." << endl;

    // The names in the database are now stored in the provided
    // vector named databaseNames

    /* TODO: Fill in the remainder of this function. */
}


/* * * * * * Test Cases * * * * * */


PROVIDED_TEST("Test removing puntuation, digits, and spaces") {
    //测试空字符串
    string str = "";
    string res = removeNonLetters(str);
    EXPECT_EQUAL(res, str);
    //测试开始非字母的
    string strr = "''Open";
    string st = removeNonLetters(strr);
    EXPECT_EQUAL(st, "Open");
    //官方测试
    string s = "O'Hara";
    string result = removeNonLetters(s);
    EXPECT_EQUAL(result, "OHara");
    s = "Planet9";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "Planet");
    s = "tl dr";
    result = removeNonLetters(s);
    EXPECT_EQUAL(result, "tldr");
}


PROVIDED_TEST("Sample inputs from handout") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
    EXPECT_EQUAL(soundex("O'Conner"), "O256");
}

PROVIDED_TEST("hanrahan is in lowercase") {
    EXPECT_EQUAL(soundex("hanrahan"), "H565");
}

PROVIDED_TEST("DRELL is in uppercase") {
    EXPECT_EQUAL(soundex("DRELL"), "D640");
}

PROVIDED_TEST("Liu has to be padded with zeros") {
    EXPECT_EQUAL(soundex("Liu"), "L000");
}

PROVIDED_TEST("Tessier-Lavigne has a hyphen") {
    EXPECT_EQUAL(soundex("Tessier-Lavigne"), "T264");
}

PROVIDED_TEST("Au consists of only vowels") {
    EXPECT_EQUAL(soundex("Au"), "A000");
}

PROVIDED_TEST("Egilsdottir is long and starts with a vowel") {
    EXPECT_EQUAL(soundex("Egilsdottir"), "E242");
}

PROVIDED_TEST("Jackson has three adjcaent duplicate codes") {
    EXPECT_EQUAL(soundex("Jackson"), "J250");
}

PROVIDED_TEST("Schwarz begins with a pair of duplicate codes") {
    EXPECT_EQUAL(soundex("Schwarz"), "S620");
}

PROVIDED_TEST("Van Niekerk has a space between repeated n's") {
    EXPECT_EQUAL(soundex("Van Niekerk"), "V526");
}

PROVIDED_TEST("Wharton begins with Wh") {
    EXPECT_EQUAL(soundex("Wharton"), "W635");
}

PROVIDED_TEST("Ashcraft is not a special case") {
    // Some versions of Soundex make special case for consecutive codes split by hw
    // We do not make this special case, just treat same as codes split by vowel
    EXPECT_EQUAL(soundex("Ashcraft"), "A226");
}

// TODO: add your test cases here


