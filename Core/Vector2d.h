#ifndef VECTEUR2D_H_
#define VECTEUR2D_H_

#include <iostream>
#include <cassert>

class Point2d;
class Segment;

class Vector2d {
public:
  Vector2d(const float& x = 0.0, const float& y = 0.0);
  Vector2d(const Segment& segment);
  Vector2d(const Point2d& A, const Point2d& B);
  Vector2d(const Vector2d& vector);
  virtual ~Vector2d(void) {}

  inline float getX() const { return _coords[0]; }
  inline float getY() const { return _coords[1]; }

  inline void setX(float x) { _coords[0] = x; }
  inline void setY(float y) { _coords[1] = y; }

  inline float operator[](int i) const { assert(i < 2); assert(-1 < i); return _coords[i]; }
  inline float& operator[](const int& i) { assert(i < 2); assert(-1 < i); return _coords[i]; }

  static Vector2d fromSegment(const Segment& segment);

  friend bool operator==(const Vector2d& vector1, const Vector2d& vector2);
  friend bool operator!=(const Vector2d& vector1, const Vector2d& vector2);

  const Vector2d& operator+(void) const;
  Vector2d operator-(void) const;

  Vector2d& operator=(const Vector2d& vector);

  friend Vector2d operator+(const Vector2d& vector1, const Vector2d& vector2);
  friend Vector2d operator-(const Vector2d& vector1, const Vector2d& vector2);
  friend Vector2d operator*(const Vector2d& vector, const float& k);
  friend Vector2d operator*(const float& k, const Vector2d& vector);
  friend float operator*(const Vector2d& vector1, const Vector2d& vector2);
  friend Vector2d operator/(const Vector2d& vector, const float& k);

  Vector2d& operator+=(const Vector2d& vector);
  Vector2d& operator-=(const Vector2d& vector);
  Vector2d& operator*=(const float& k);
  Vector2d& operator/=(const float& k);

  float norm(void) const;
  float squaredNorm(void) const;
  Vector2d normalized(void) const;
  Vector2d& normalize(void);

  static float determinant(const Vector2d& vector1, const Vector2d& vector2);
  static bool areColinear(const Vector2d& vector1, const Vector2d& vector2);
  static bool areOrthogonal(const Vector2d& vector1, const Vector2d& vector2);
  static float angle(const Vector2d& vector1, const Vector2d& vector2);

  friend std::ostream& operator<<(std::ostream& os, const Vector2d& vector);

private:
  float _coords[2];
};

#endif /* VECTEUR2D_H_ */
