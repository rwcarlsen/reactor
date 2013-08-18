
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "sdl/surface.h"
#include "sdl/color.h"
#include "phys/material.h"

namespace phys {

class Object {
 public:
  typedef struct {
    int x, y, w, h;
  } Rect;

  Object(Material* m, Rect bounds, sdl::Color c = sdl::Color::white()) : m_(m), r_(bounds) {
    surf_ = new sdl::Surface(bounds.w, bounds.h);
    surf_->FillRect(NULL, c);
  }

  bool Contains(int x, int y) const {
    return (x >= r_.x) && (x < r_.x + r_.w) &&
           (y >= r_.y) && (y < r_.y + r_.h);
  };

  Material* material() {
    return m_;
  }

  Rect rect() {
    return r_;
  }

  sdl::Surface* surface() const {
    return surf_;
  }

  void Shift(int dx, int dy) {
    r_.x += dx;
    r_.y += dy;
  };

 private:
  Material* m_;
  Rect r_;
  sdl::Surface* surf_;
};

} // namespace phys

#endif
