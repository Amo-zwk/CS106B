/*
 * CS 106B, Julie Zelenski
 *
 * This file declares a class named Ball.
 * A class is a template or blueprint for a new type of object.
 *
 * Each Ball object represents information about a ball.
 * A ball has a location (x,y), velocity (vx, vy) and id number.
 */
#pragma once
#include "gwindow.h"
#include "gfont.h"
#include <string>

class Ball {

public:
    Ball(int id, GWindow* w);
    void draw();
    void move();

private:
    int _x;
    int _y;
    int _vx;
    int _vy;
    int _id;
    GWindow* _window;
};

static void drawCenteredText(GWindow *win, double x, double y, double width, double height, std::string text)
{
    struct { int width, height; } bounds;
    int font_size = 1;
    do {
        std::string font_desc = "Helvetica-" + integerToString(++font_size);
        win->setFont(font_desc);
        QFontMetrics metrics(GFont::toQFont(font_desc));
        bounds = { metrics.horizontalAdvance(QString::fromStdString(text)), metrics.ascent() };
    } while (bounds.height < height/2 && bounds.width < width/2);
    win->drawString(text, x + (width-bounds.width)/2, y + height - (height-bounds.height)/2);
}
