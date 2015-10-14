#ifndef FUEL_H_
#define FUEL_H_

#include <math.h>
#include <map>

#include "phys/object.h"

namespace phys {

class Fuel : public Object {
 public:
  Fuel(double scat_p, double fiss_p, double avg_yield)
    : avg_yield_(avg_yield), nom_fiss_prob_(fiss_p), scat_prob_(scat_p) { };

  Fuel(const Fuel* f) : Object(f) {
    avg_yield_ = f->avg_yield_;
    nom_fiss_prob_ = f->nom_fiss_prob_;
    scat_prob_ = f->scat_prob_;
    rand_gen_ = f->rand_gen_;
    uniform_ = f->uniform_;
  }

  virtual double fiss_prob(double speed) {
    return Neutron::kMinSpeed / speed * nom_fiss_prob_;
  };

  virtual double scatter_prob(double speed) {
    return scat_prob_;
  };

  virtual bool React(Rxn rx, double x, double y) {
    int fuel_per_px = 10000;
    if (rx != RxFission) {
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

  virtual Neutron::V scat_v(Neutron::V v, double speed) {
    double theta = uniform_(rand_gen_) * 2 * kPi;
    double vy = speed * std::sin(theta);
    double vx = speed * std::cos(theta);
    return Neutron::V {vx, vy};
  };

  virtual int fiss_yield() {
    return (int)((uniform_(rand_gen_) + 1) * avg_yield_);
  };

 private:
  std::map<std::pair<int, int>, int > voids_;
  double avg_yield_;
  double nom_fiss_prob_;
  double scat_prob_;
  std::ranlux48_base rand_gen_;
  std::uniform_real_distribution<> uniform_;
};

}

#endif
