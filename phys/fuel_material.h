
#ifndef FUEL_H_
#define FUEL_H_

#include <math.h>

#include "phys/material.h"

namespace phys {

class Fuel : public Material {
 public:
  Fuel(double scat_p, double fiss_p, double avg_yield)
    : avg_yield_(avg_yield), nom_fiss_prob_(fiss_p), scat_prob_(scat_p) { };

  virtual double fiss_prob(double speed) {
    return Neutron::kMinSpeed / speed * nom_fiss_prob_;
  };

  virtual double scatter_prob(double speed) {
    return scat_prob_;
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
  double avg_yield_;
  double nom_fiss_prob_;
  double scat_prob_;
  std::ranlux48_base rand_gen_;
  std::uniform_real_distribution<> uniform_;
};

}

#endif
