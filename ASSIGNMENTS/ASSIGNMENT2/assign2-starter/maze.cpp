#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"

using namespace std;

/*
 * 这个函数的意思是,找出cur这个二维坐标上下左右四个方向可以遍历的点,即是通道的点
 */

//generateValidMoves函数
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1}; //偏移量
    int x = cur.row, y = cur.col; //获取cur的行列坐标
    int mr = maze.numRows(), mc = maze.numCols(); //获取地图的行数和列数
    for(int i = 0; i < 4; i++) {
        int a = x + dx[i], b = y + dy[i];//获取新坐标
        if(a >=0 && a < mr && b >=0 && b < mc && maze[a][b] == true) { //边界条件(不能超过地图大小以及要满足当前位置是通道)
            GridLocation newcur = {a,b};
            neighbors.add(newcur); //加入集合里面
        }
    }
    return neighbors; //返回答案
}

//validatePath函数
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {

    Set<GridLocation> visited; //定义一个ADT存放visited(记录路径,避免重复)

    //迷宫入口为{0,0}

    //备份
    Stack<GridLocation> copypath = path;

    //入口
    GridLocation mazeEnter = {0,0};

    //获取路径的起点(即栈底,即迷宫入口)
    while(copypath.size() != 1 && (!copypath.isEmpty())){ //要判断path为空的情况,避免死循环
        copypath.pop(); //出栈
    }

    //路径的起点必须位于原点(0,0)
    if (copypath.peek() != mazeEnter) {
        error("路径的起点必须位于原点(0,0)");
    }

    //迷宫出口
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};

    //路径的终点必须位于迷宫出口右下角,坐标为(maze.numRows()-1,  maze.numCols()-1)
    if (path.peek() != mazeExit) {
        error("路径的终点必须位于迷宫出口右下角");
    }

    //循环
    while(!path.isEmpty()) {
        Set<GridLocation> neighbors = generateValidMoves(maze,path.pop()); //取出栈顶并且计算generateValidMoves
        //判断是否在visited里面
        if(( !path.isEmpty()) && (!neighbors.contains(path.peek())) ) { //如果栈不为空并且下一个位置不在neighbors里面
            error("下一个位置必须在通道上");
        }
        else if(( !path.isEmpty()) && (neighbors.contains(path.peek())) && !(visited.contains(path.peek())) ) { //如果栈不为空并且下一个位置在neighbors里面,并且没被visited添加过
            visited.add(path.peek()); //就把下一个位置加入到路径里
        }
        else if(( !path.isEmpty()) && (neighbors.contains(path.peek())) && (visited.contains(path.peek())) ) { //如果栈不为空并且下一个位置在neighbors里面,并且被visited添加过,说明重复了
            error("道路重复走过了");
        }
    }

    //循环结束,此时栈为空

}

//helperFuntion函数
Set<GridLocation> helperFuntion(Stack<GridLocation> path) {
    Set<GridLocation> visited;
    while (!path.isEmpty()) {
        visited.add(path.pop());
    }
    return visited;
}

//solveMaze函数
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    //官方定义的变量
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    //迷宫入口
    Stack<GridLocation> Enter = {{0,0}};
    //迷宫出口
    GridLocation Exit = {maze.numRows()-1,maze.numCols()-1};

    //定义一个队列变量q
    Queue<Stack<GridLocation>> q;
    //将起点入队
    q.enqueue(Enter);
    //当队列不为空
    while(!q.isEmpty()) {
        path = q.dequeue(); //取出队首元素
        MazeGraphics::highlightPath(path, "GREEN", 100);
        if(path.peek()==Exit) { //搜索结束条件
            break; //终止循环
        }
        Set<GridLocation> neighbours = generateValidMoves(maze,path.peek()); //根据当前路径的末端，找出所有可以继续前进的位置。需要注意的是，不要回头
        for(auto neighbour: neighbours) { //遍历所有可以走的点
            //选出没有走过的点即可,因为generateValidMoves函数得到的就是可以走的点并且是通道的点
            if(!helperFuntion(path).contains(neighbour)) { //如果没走过,加入队列里面,并设置为已访问
                auto copyPath = path;
                copyPath.push(neighbour);
                q.enqueue(copyPath);//入队
            }
        }
    }
    return path;
}

//readMazeFile函数
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

//readSolutionFile函数
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

//generateValidMoves函数官方测试用例1
PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
//generateValidMoves函数官方测试用例2
PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}
//generateValidMoves函数官方测试用例3
PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1,0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

//validatePath函数官方测试用例1
PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

//validatePath函数官方测试用例2
PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

//validatePath函数官方测试用例3
PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

//validatePath函数官方测试用例4
PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} }; //error 地图终点不等于路径的终点
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} }; //error 地图起点不等于路径的起点
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} }; //路径穿墙了
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} }; //路径走八字了,实际只能四字
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} }; //走重复路径了

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

//solveMaze函数官方测试用例1
PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// 请在这里添加你的额外测试用例

//generateValidMoves函数测试用例1
PROVIDED_TEST("Students tests on generateValidMoves") {
    Grid<bool> maze = {{true, false, false},
                       {false, false, false},
                       {false, false, false}};
    GridLocation side = {0, 0};
    Set<GridLocation> expected = {}; //表示空集合

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}
//generateValidMoves函数测试用例2
PROVIDED_TEST("Students tests on generateValidMoves") {
    Grid<bool> maze = {{false, false, false},
                       {false, false, false},
                       {false, false, true}};
    GridLocation side = {2, 2};
    Set<GridLocation> expected = {}; //表示空集合

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}
//generateValidMoves函数测试用例3
PROVIDED_TEST("Students tests on generateValidMoves") {
    Grid<bool> maze = {{true, true, true},
                       {false, true, false},
                       {false, false, false}};
    GridLocation center = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
//generateValidMoves函数测试用例4
PROVIDED_TEST("Students tests on generateValidMoves") {
    Grid<bool> maze = {{true, true, true}};
    GridLocation center = {0, 1};
    Set<GridLocation> expected = {{0, 0}, {0, 2}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}
//generateValidMoves函数测试用例5
PROVIDED_TEST("Students tests on generateValidMoves") {
    Grid<bool> maze = {{true}};
    GridLocation center = {0, 0};
    Set<GridLocation> expected = {};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

//validatePath函数测试用例1
PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, true},
                       {false, true}};
    Stack<GridLocation> soln = { {0 ,0}, {0, 1}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

//validatePath函数测试用例2
PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {false, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, soln));
}

//validatePath函数测试用例3
PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
