
#include <raylib.h>

#ifndef VEC2
#define VEC2

struct Vec2 : public Vector2 {

  bool operator==(const Vec2 v1) const {
    bool res = false;
    if ((v1.x == x) && (v1.y == y)) {
      res = true;
    }
    return res;
  };
  bool operator!=(const Vec2 v1) const { return !(v1 == *this); };
  Vec2 operator+=(const Vec2 v) const {
    Vec2 nv;
    nv.x = x + v.x;
    nv.y = y + v.y;
    return nv;
  };

  Vec2 operator-(const Vec2 v) {
    Vec2 nv(x - v.x, y - v.y);
    return nv;
  };
  Vec2 &operator=(Vec2 other) {
    x = other.x;
    y = other.y;
    return *this;
  };

  Vec2 operator+(const Vec2 v) const {
    Vec2 nv = {x + v.x, y + v.y};
    return nv;
  };
  Vec2 operator*(const float v) {
    Vec2 nv = {v * x, v * y};
    return nv;
  };
  Vec2 operator*=(const float v) {
    Vec2 nv = {v * x, v * y};
    return nv;
  };
  Vector2 as_vector2() const {
    Vector2 v = {x, y};
    return v;
  };
  Vec2(Vector2 v) {
    x = v.x;
    y = v.y;
  };
  Vec2() {
    x = 0;
    y = 0;
  }
  Vec2(float x, float y) {
    this->x = x;
    this->y = y;
  }
};

#endif
