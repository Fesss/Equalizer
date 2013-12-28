
#include "Point.h"

Point::Point() {
    x=0;
    y=0;
}
Point::Point(float x, float y){
    this->x=x;
    this->y=y;
}
Point::Point(int x, int y){
    this->x=x;
    this->y=y;
}
Point::Point(const Point& point){
    x=point.x;
    y=point.y;
}
int Point::getX() const{
    return x;
}
int Point::getY() const{
    return y;
}
void Point::setX(float x){
    this->x=x;
    
}
void Point::setY(float y){
    this->y=y;
}


