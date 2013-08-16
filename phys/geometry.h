
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "phys/material.h"

namespace phys {

class Geometry {
 public:
  typedef struct {
    int x, y, w, h;
  } Rect;

  void AddMaterial(Material* m, Rect bounds) {
    mats_.push_back(m);
    rects_.push_back(Rect2 {bounds.x, bounds.y, bounds.x + bounds.w, bounds.y + bounds.h});
  };

  Material* MatFor(int x, int y) {
    for (int i = 0; i < mats_.size(); ++i) {
      Rect2 r = rects_[i];
      if (x >= r.x1 && x < r.x2 && y >= r.y1 && y < r.y2) {
        return mats_[i];
      }
    }
    return &blank_;
  };

 private:
  typedef struct {
    int x1, y1, x2, y2;
  } Rect2;

  Material blank_;
  std::vector<Material*> mats_;
  std::vector<Rect2> rects_;
};

} // namespace phys

#endif
