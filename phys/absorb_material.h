#ifndef ABSORB_MATERIAL_H_
#define ABSORB_MATERIAL_H_

#include <math.h>

#include "phys/object.h"

namespace phys {

// all probabilities given per unit length
class Absorber : public Object {
 public:
  Absorber(double max_p_absorb)
    : p_absorb(max_p_absorb) { }

  Absorber(const Absorber* f) : Object(f) {
    p_absorb = f->p_absorb;
  }

  virtual double absorb_prob(double speed) {
    return Neutron::kMinSpeed / speed * p_absorb;
  };

  virtual bool React(Rxn rx, double x, double y) {
    int fuel_per_px = 25;
    if (rx != RxAbsorb) {
      return true;
    }

    Rect r = rect();
    double relx = x - r.x;
    double rely = y - r.y;
    std::pair<int, int> p = std::make_pair((int)(relx), (int)(rely));
    if (voids_[p] > fuel_per_px) {
      return false;
    }

    if (voids_.count(p) == 0) {
      voids_[p] = 0;
    }

    voids_[p]++;
    if (voids_[p] > fuel_per_px) {
      SDL_Point pt = {(int)relx, (int)rely};
      surf_->DrawPoint(pt, sdl::Color::gray());
    }

    return true;
  };

  std::map<std::pair<int, int>, int > voids_;
  double p_absorb;
};

} // namespace phys

#endif
