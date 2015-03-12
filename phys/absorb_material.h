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


  double p_absorb;
};

} // namespace phys

#endif
