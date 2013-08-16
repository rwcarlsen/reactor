
#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "phys/neutron.h"

namespace phys {

// all probabilities given per unit length
class Material {
 public:
  virtual double absorb_prob(double speed) {
    return 0;
  };

  virtual double fiss_prob(double speed) {
    return 0;
  };

  virtual double scatter_prob(double speed) {
    return 0;
  };

  virtual Neutron::V scat_v(Neutron::V v) {
    return v;
  };

  virtual int fiss_yield() {
    return 0;
  };
};

} // namespace phys

#endif
