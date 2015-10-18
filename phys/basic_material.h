
#ifndef BASIC_MATERIAL_H_
#define BASIC_MATERIAL_H_

#include <math.h>

#include "phys/object.h"

namespace phys {

// all probabilities given per unit length
class BasicMaterial : public Object {
 public:
  BasicMaterial(double pa, double pf, double ps, double sf, double y)
    : uniform_(0, 2 * kPi),
      p_absorb(pa),
      p_fiss(pf),
      p_scatter(ps),
      scat_frac(sf),
      yield(y) { };

  BasicMaterial(const BasicMaterial* f) : Object(f) {
    p_absorb = f->p_absorb;
    p_fiss = f->p_fiss;
    p_scatter = f->p_scatter;
    scat_frac = f->scat_frac;
    yield = f->yield;
    rand_gen_ = f->rand_gen_;
    uniform_ = f->uniform_;
  }

  virtual BasicMaterial* clone() { return new BasicMaterial(&(*this)); };

  virtual double absorb_prob(double speed) {
    return p_absorb;
  };

  virtual double fiss_prob(double speed) {
    return p_fiss;
  };

  virtual double scatter_prob(double speed) {
    return p_scatter;
  };

  virtual Neutron::V scat_v(Neutron::V v, double speed) {
    double theta = uniform_(rand_gen_);
    double vy;
    double vx;
    if (speed * scat_frac > Neutron::kMinSpeed) {
      vy = speed * scat_frac * std::sin(theta);
      vx = speed * scat_frac * std::cos(theta);
    } else {
      vy = Neutron::kMinSpeed * std::sin(theta);
      vx = Neutron::kMinSpeed * std::cos(theta);
    }
    return Neutron::V {vx, vy};
  };

  virtual int fiss_yield() {
    return yield;
  };

  double p_absorb;
  double p_fiss;
  double p_scatter;
  double scat_frac;
  int yield;

 private:
  std::ranlux48_base rand_gen_;
  std::uniform_real_distribution<> uniform_;
};

} // namespace phys

#endif
