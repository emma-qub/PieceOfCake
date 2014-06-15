#ifndef POINT2D_H
#define POINT2D_H

#include <iostream>
#include <QDebug>

class Vector2d;

class Point2d {
public:
  Point2d(float x = 0.0, float y = 0.0);
  Point2d(int xMin, int xMax, int yMin, int yMax);
  Point2d(const Point2d& point2d);
  virtual ~Point2d(void) {}

  inline float getX(void) const { return _coords[0]; }
  inline float getY(void) const { return _coords[1]; }

  inline void setX(const float& x) { _coords[0] = x; }
  inline void setY(const float& y) { _coords[1] = y; }

  void move(const float& x = 0.0, const float& y = 0.0);

  void symetry(void);

  void homothetie(const Point2d& origin, float scale);
  Point2d applyVector(const Vector2d& vector) const;

  Point2d operator/(const float& k) const;
  Point2d& operator/=(const float& k);

  friend Point2d operator+(const Point2d& point1, const Point2d& point2);
  Point2d& operator+=(const Point2d& point);

  friend Vector2d operator-(const Point2d& point1, const Point2d& point2);

  Point2d& operator=(const Point2d& point);

  friend bool operator==(const Point2d& point1, const Point2d& point2);
  friend bool operator!=(const Point2d& point1, const Point2d& point2);
  friend bool operator<(const Point2d& point1, const Point2d& point2);
  friend bool operator<=(const Point2d& point1, const Point2d& point2);
  friend bool operator>(const Point2d& point1, const Point2d& point2);
  friend bool operator>=(const Point2d& point1, const Point2d& point2);

  static float distance(const Point2d& point1, const Point2d& point2);

  friend std::ostream& operator<<(std::ostream& os, const Point2d& point);
  friend QDebug operator<<(QDebug d, const Point2d& point);

private:
  float _coords[2];
};

#endif // POINT2D_H
