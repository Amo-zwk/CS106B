#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "gridlocation.h"
#include "grid.h"
#include "set.h"
#include "lexicon.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * 一个单词的分数根据其长度进行评分。
 * 根据上述标准，具有 3 个或更少字符的单词无效，因此记零分。
 * 一个 4 个字母的单词获得 1 分；
 * 一个 5 个字母的单词获得 2 分；
 * 6 个字母的单词获得 3 分；
 * 以此类推。
 */

//points(sextet) -----> return 1 + points(sexte) -----> return 3
//points(sexte) -----> return 1 + points(sext) -----> return 2
//points(sext) -----> return 1 + points(sex) -----> return 1
//points(sex) -----> return 0

int points(string str) {
    if(str.size() <= 3) return 0;
    str.pop_back();
    return 1 + points(str);
}

/*
 * NextLocations 函数
 */
Set<GridLocation> nextLocations(Grid<char>& board, Vector<GridLocation>& mark) {

    GridLocation cur = mark[mark.size() - 1]; //得到要遍历的点
    Set<GridLocation> neighbors; // neighbors得到附近可以遍历的点

    /*
     * 两个for用来遍历上下左右和对角线得到neighbors
     * {-1,-1} {-1,0} {-1,1}
     * {0,-1} {0,0} {0,1}
     * {1,-1} {1,0} {1,1}
     */
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            GridLocation next = GridLocation(cur.row + i, cur.col + j); //相邻的点的坐标
            if (board.inBounds(next)) { //如果坐标在地图内
                bool exist = false; // exist的作用是什么？
                //这里的for循环是用来排除cur那个自身点以及相邻的点的坐标不可以重复出现或者不可以是本身
                //所以为什么mark要用vector,因为要每找到一个字母,都要记录并且用来做for循环判断是否走了第二次或者是是否是本身
                for (const GridLocation& loc : mark) {
                    if (loc == next) exist = true;

                }
                //如果相邻点不是本身或者不是走过的点(走过的点都存在向量mark里面),就加入到neighbors里
                if (!exist) neighbors.add(next);
            }
        }
    }
    return neighbors;
}

/*
 * vecToStr 函数
 */
string vecToStr(Grid<char> & board, Vector<GridLocation> & sofar) {

    string res = "" ;
    for(const GridLocation & loc : sofar)  { //为什么这里要遍历,不可以直接res.append 答:因为每增加一个字母,都要append,不是只增加一次
        res.append(charToString(board.get(loc))); //根据给出的坐标,得到当前位置的字母
    }
    return res;
}


/*
 * scoreBoardHelper 函数
 */
int scoreBoardHelper(Grid<char>& board, Lexicon& lex, Vector<GridLocation>& mark,Set<string>& visited)
{

    string str = vecToStr(board, mark); //思考: 这一行的作用是什么？怎么想到的 作用:把当前坐标所表示的字母用一个字符串str来接收
    // 1. Base Case: 无此前缀，清除无效分支
    if (!lex.containsPrefix(str)) {
        return 0;
    }
    // 2. Base Case: 无更多位置，作最终判断
    Set<GridLocation> neighbors = nextLocations(board, mark); //思考: 这一行的作用是什么？为什么要传入mark 作用:得到相邻的点,然后对相邻的点递归操作
    if (neighbors.isEmpty()) { //先不看这个判断
        if (lex.contains(str) && !visited.contains(str)) {
            visited.add(str); //如果形成一个单词了,则加入到visited里面,避免重复添加
            return points(str);
        }
        return 0;
    } else {
        int point = 0;
        // 3. Special Case：中间过程可以构成计分单词
        if (str.size() > 3 && lex.contains(str) && !visited.contains(str)) {
            visited.add(str); //如果形成一个单词了,则加入到visited里面,避免重复添加
            point += points(str);
        }
        // 4. Recursive Case：继续探索更多分支
        for (const GridLocation& loc : neighbors) {
            mark.add(loc); //mark是用来保存就是从遍历的第一个单词开始记录直到结束为止
            point += scoreBoardHelper(board, lex, mark, visited); //然后从相邻的点开始继续递归,每次从相邻的点开始递归都要转换成str,所以就有string str = vecToStr(board, mark)
            mark.remove(mark.size() - 1); //恢复状态
        }
        return point;
    }
}

/*
 * scoreBoard 函数
 */
int scoreBoard(Grid<char>& board, Lexicon& lex) {

    int total = 0;
    Set<string> visited;

    /*
     * 两个for遍历board上每一个点
     */
    for(int row = 0 ; row < board.numRows() ; row++ )  {
        for(int col = 0 ; col < board.numCols() ; col++ )  {
            /*
             * 用mark存地图上的每一个点 思考: 为什么要用Vector 而不是直接GridLocation？
             */
            Vector<GridLocation > mark = {{row , col}};
            /*
             * 累加每个点的total 思考: 为什么需要mark 和 visited参数？
             * 答: mark是用来记录一个完整的单词从第一个字母到最后一个字母,所以要用vector
             * 答: visited的话,则用来记录已经计算过total的单词,避免重复计算
             */
            total += scoreBoardHelper(board,lex,mark,visited);
         }
    }

    /*
     * 返回总数
     */
    return total;
}

/* * * * * * Test Cases * * * * * */

/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

//scoreBoard函数官方测试1
PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Load shared Lexicon, confirm number of words") {
    Lexicon lex = sharedLexicon();
    EXPECT_EQUAL(lex.size(), 127145);
}

//points函数官方测试
PROVIDED_TEST("Test point scoring") {
    EXPECT_EQUAL(points("and"), 0);
    EXPECT_EQUAL(points("quad"), 1);
    EXPECT_EQUAL(points("quint"), 2);
    EXPECT_EQUAL(points("sextet"), 3);
    EXPECT_EQUAL(points("seventh"), 4);
    EXPECT_EQUAL(points("supercomputer"), 10);
}

//scoreBoard函数官方测试2
PROVIDED_TEST("Test scoreBoard, board contains no words, score of zero") {
    Grid<char> board = {{'B','C','D','F'}, //no vowels, no words
                        {'G','H','J','K'},
                        {'L','M','N','P'},
                        {'Q','R','S','T'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 0);
}

//scoreBoard函数官方测试3
PROVIDED_TEST("Test scoreBoard, board contains one word, score of 1") {
    Grid<char> board = {{'C','_','_','_'},
                        {'Z','_','_','_'},
                        {'_','A','_','_'},
                        {'_','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

//scoreBoard函数官方测试4
PROVIDED_TEST("Test scoreBoard, alternate paths for same word, still score of 1") {
    Grid<char> board = {{'C','C','_','_'},
                        {'C','Z','C','_'},
                        {'_','A','_','_'},
                        {'R','_','R','_'}};
    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 1);
}

//scoreBoard函数官方测试5
PROVIDED_TEST("Test scoreBoard, small number of words in corner of board") {
    Grid<char> board = {{'L','I','_','_'},
                        {'M','E','_','_'},
                        {'_','S','_','_'},
                        {'_','_','_','_'}};
    Set<string> words = {"SMILE", "LIMES", "MILES", "MILE", "MIES", "LIME", "LIES", "ELMS", "SEMI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()),  2 + 2 + 2 + 1 + 1 + 1 + 1 + 1 + 1);
}

//scoreBoard函数官方测试6
PROVIDED_TEST("Test scoreBoard, full board, small number of words") {
    Grid<char> board = {{'E','Z','R','R'},
                        {'O','H','I','O'},
                        {'N','J','I','H'},
                        {'Y','A','H','O'}};
    Set<string> words = { "HORIZON", "OHIA", "ORZO", "JOHN", "HAJI"};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 4 + 1 + 1 + 1 + 1);
}

PROVIDED_TEST("Test scoreBoard, full board, medium number of words") {
    Grid<char> board = {{'O','T','H','X'},
                        {'T','H','T','P'},
                        {'S','S','F','E'},
                        {'N','A','L','T'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 76);
}

//scoreBoard函数官方测试7
PROVIDED_TEST("Test scoreBoard, full board, large number of words") {
    Grid<char> board = {{'E','A','A','R'},
                        {'L','V','T','S'},
                        {'R','A','A','N'},
                        {'O','I','S','E'}};

    EXPECT_EQUAL(scoreBoard(board, sharedLexicon()), 234);
}
