
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "phys/material.h"

namespace phys {

class Object {
 public:
  typedef struct {
    int x, y, w, h;
  } Rect;

  Object(Material* m, Rect bounds) : m_(m), r_(bounds) { }

  bool Contains(int x, int y) const {
    return (x >= r_.x) && (x < r_.x + r_.w) &&
           (y >= r_.y) && (y < r_.y + r_.h);
  };

  Material* material() {
    return m_;
  }

  void Shift(int dx, int dy) {
    r_.x += dx;
    r_.y += dy;
  };

 private:
  Material* m_;
  Rect r_;
};

} // namespace phys

#endif
