#pragma once
namespace pt{
class Point
{
public:

	Point(void);
	Point(float x, float y);
	//Point(Point &const point){
	//	this->x=point.getX();
	//	this->y=point.getY();
	//}
    ~Point(void);
	float getX();
    float getY();
    void setp(float ax, float ay);
    void setp(Point p);
    void setX(float x);
    void setY(float y);
	float x;
    float y;
private:

};}

