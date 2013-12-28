

#ifndef POINT_H
#define	POINT_H

class Point {
public:
    Point();
    Point(float, float);
    Point(int, int);
    Point(const Point&);
    int getX() const;
    int getY() const;
    void setX(float);
    void setY(float);
private:
    float x;
    float y;
    

};

#endif	/* POINT_H */

