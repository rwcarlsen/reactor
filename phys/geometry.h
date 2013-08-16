
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "phys/material.h"

namespace phys {

class Geometry {
 public:
  typedef struct{int x, y, w, h;} Rect;

  Geometry() : geom_(10000000) { };

  void AddMaterial(Material* m, Rect bounds) {
    mats_.push_back(m);
    rects_.push_back(bounds);
  };

  void Build() {
    for (int i = 0; i < mats_.size(); ++i) {
      Material* m = mats_[i];
      Rect r = rects_[i];
      for (int x = r.x; x < r.x + r.w; ++x) {
        for (int y = r.y; y < r.y + r.h; ++y) {
          geom_[x][y] = m;
        }
      }
    }
  };

  Material* MatFor(int x, int y) {
    if (geom_.count(x) == 0 || geom_[x].count(y) == 0) {
      return &blank_;
    }
    return geom_[x][y];
  };

 private:
  Material blank_;
  std::vector<Material*> mats_;
  std::vector<Rect> rects_;
  std::unordered_map<int, std::unordered_map<int, Material*> > geom_;
};

} // namespace phys

#endif
