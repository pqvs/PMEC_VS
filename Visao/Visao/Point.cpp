#include "Point.h"


pt::Point::Point() {
}
pt::Point::~Point() {
}

pt::Point::Point(float x, float y) {
    this->x = x;
    this->y = y;
}

float pt::Point::getX() {
    return this->x;
}

float pt::Point::getY() {
    return this->y;
}

void pt::Point::setX(float x){
    this->x=x;
}
void pt::Point::setY(float y){
    this->y=y;
}
void pt::Point::setp(float ax, float ay) {
    this->x = ax;
    this->y = ay;
}
void pt::Point::setp(Point Point) {
    this->x = Point.getX();
    this->y = Point.getY();
}
