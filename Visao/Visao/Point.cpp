#include "Point.h"


pt::Point::Point() {
}
pt::Point::~Point() {
}

pt::Point::Point(double x, double y) {
    this->x = x;
    this->y = y;
}

double pt::Point::getX() {
    return this->x;
}

double pt::Point::getY() {
    return this->y;
}

void pt::Point::setX(double x){
    this->x=x;
}
void pt::Point::setY(double y){
    this->y=y;
}
void pt::Point::setp(double ax, double ay) {
    this->x = ax;
    this->y = ay;
}
void pt::Point::setp(Point Point) {
    this->x = Point.getX();
    this->y = Point.getY();
}
