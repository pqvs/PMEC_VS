#pragma once
namespace pt{
class Point
{
public:

	Point(void);
	Point(double x, double y);
	//Point(Point &const point){
	//	this->x=point.getX();
	//	this->y=point.getY();
	//}
    ~Point(void);
	double getX();
    double getY();
    void setp(double ax, double ay);
    void setp(Point p);
    void setX(double x);
    void setY(double y);
	double x;
    double y;
private:

};}

