
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "material.h"

namespace phys {

class Rect {
 public:
  int x, y, w, h;
};

class Geometry {
 public:
  Geometry() : geom_(10000000) { };

  void AddMaterial(Material* m, Rect bounds) {
    mats_.push_back(m);
    rects_.push_back(bounds);
  };

  void Build() {
    for (int i = 0; i < mats_.size(); ++i) {
      auto m = mats_[i];
      auto r = rects_[i];
      for (int x = r.x; x < r.x + r.w; ++x) {
        for (int y = r.y; y < r.y + r.h; ++y) {
          geom_[x][y] = m;
        }
      }
    }
  };

  const Material& MatFor(int x, int y) {
    return *geom_[x][y];
  };

 private:
  std::vector<Material*> mats_;
  std::vector<Rect> rects_;
  std::unordered_map<int, std::unordered_map<int, Material*> > geom_;
};

} // namespace phys

#endif
