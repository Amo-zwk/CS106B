#include <iostream>    // for cout, endl
#include "recursion.h"
#include "testing/SimpleTest.h"
using namespace std;

/**
 * tips:
 * - 其中一个关键部分是确定每个三角形顶点的位置，仔细思考应该很容易做到。
 * - 大的外三角形的每一边的中点是较小的内三角形的一个角。给定一个线段，其端点是 {x1, y1} 和 {x2, y2}，则中点是 {(x1 + x2) / 2, (y1 + y2) / 2}
 * - 三角形的三个顶点可以任意放置。它们不一定形成一个正三角形，底边也不一定与窗口底部平行。选择哪个顶点为 one，two，three 完全是任意的
 * - GPoint 结构用于指定图形窗口中的 x，y 坐标，请查阅 GPoint 的文档。
 */

void fillBlackTriangle(GWindow& window, GPoint one, GPoint two, GPoint three) {
    window.setColor("black");
    window.fillPolygon( {one, two, three} );
}


//order等于2
//order等于1 (1,1/2,1/2) (1/2,1,1/2) (1/2,1/2,1)
//order等于0 (1,1,1)
int drawSierpinskiTriangle(GWindow& window, GPoint one, GPoint two, GPoint three, int order) {

    //Order-n阶的Sierpinski三角形是由3个Order-(n-1)阶的Sierpinski三角形组成,它们的边长是原始边长的一半，彼此之间顶点相连排列
    //one two three分别是Sierpinski三角形的三个顶点

    //Base Case
    if(!order) {
        fillBlackTriangle(window,one,two,three);
        return 1;
    }

    //三个顶点

    GPoint p4;
    GPoint p5;
    GPoint p6;

    p4.x = (one.x+two.x)/2;
    p4.y = (one.y+two.y)/2;

    p5.x = (three.x+two.x)/2;
    p5.y = (three.y+two.y)/2;

    p6.x = (three.x+one.x)/2;
    p6.y = (three.y+one.y)/2;

    return
    drawSierpinskiTriangle(window, one, p4, p6, order-1)+
    drawSierpinskiTriangle(window, p4, two, p5, order-1)+
    drawSierpinskiTriangle(window, p6, p5, three, order-1);
}

/* * * * * * Test Cases * * * * * */

/*
 * Do not modify the code beyond this line! There are no
 * unit tests for this problem. You should do all testing
 * via the interactive demo.
 */

PROVIDED_TEST("Test fractal drawing interactively using graphical demo") {
    runInteractiveDemo();
}

