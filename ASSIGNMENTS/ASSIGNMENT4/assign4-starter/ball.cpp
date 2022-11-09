/*
 * CS 106B, Julie Zelenski
 *
 * This file provides the implementation for the Ball class
 * constructor and member functions declared in ball.h.
 */
#include "ball.h"
#include "random.h"
#include "testing/SimpleTest.h"
using namespace std;

// program constants used by the Ball class
static const int SIZE = 50;
static const int MIN_VELOCITY = -5;
static const int MAX_VELOCITY = 5;


Ball::Ball(int id, GWindow* w) {
//    setRandomSeed(1);
    _window = w;
    _id = id;
    _x = randomInteger(0, _window->getWidth());
    _y = randomInteger(0, _window->getHeight());
    _vx = randomInteger(MIN_VELOCITY, MAX_VELOCITY);
    _vy = randomInteger(MIN_VELOCITY, MAX_VELOCITY);
}

void Ball::draw() {
    _window->setColor("Blue");
    _window->fillOval(_x, _y, SIZE, SIZE);
    _window->setColor("White");
    drawCenteredText(_window, _x, _y, SIZE, SIZE, integerToString(_id));
}

void Ball::move() {
    _x += _vx;
    _y += _vy;
    // if outside left or right edge, bounce horizontally
    if (_x < 0 || ((_x + SIZE > _window->getWidth()) && (_vx > 0))){
        _vx = -_vx;
    }
    // if outside top or bottom edge, bounce vertically
    if (_y < 0 || ((_y + SIZE > _window->getHeight()) && (_vy > 0))) {
        _vy = -_vy;
    }
}


/* * * * * * Test Cases * * * * * */

PROVIDED_TEST("Animate bouncing balls in window for a while") {
    setRandomSeed(8);

    GWindow window;
    window.setTitle("Bouncing Balls");
    window.setLocation(0, 0);
    window.setCanvasSize(500, 250);
    window.setResizable(false);
    window.setAutoRepaint(false);

    // Construct many random ball objects, store all balls in a vector
    Vector<Ball> allBalls;
    for (int i = 0; i < 9; i++) {
         Ball ball(i, &window);
         allBalls.add(ball);
    }

    // animation loop: move/draw all balls in each time step
    for (int i = 0; i < 200; i++) {
        window.clearCanvas();
        for (int i = 0; i < allBalls.size(); i++) {
            allBalls[i].move();
            allBalls[i].draw();
        }
        window.repaint();
        pause(10); // very brief pause
    }
    window.close();
}
