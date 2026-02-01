#include <iostream>
class Point2D {
 public:
  Point2D(const int& x = 0, const int& y = 0) : x_(x) , y_(y) {}
  int getX() {return x_;}
  int getY() {return y_;}
  void setX(const int& x) { x_ = x;}
  void setY(const int& y) { y_ = y;}
 private:
  int x_, y_;
};
