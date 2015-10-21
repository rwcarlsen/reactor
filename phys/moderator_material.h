#ifndef MODERATOR_H_
#define MODERATOR_H_

#include <math.h>
#include <map>

#include "phys/object.h"

namespace phys {

class Moderator : public BasicMaterial {
 public:
  // expand_coeff is the number of neutrons at which, if inside the moderator
  // material, the moderator actually has the given scatter probability.  If
  // expand_coeff is 100, then the moderator will have twice the given scatter
  // probability if there are 50 neutrons inside it and half the given scatter
  // probability if there are 200 neutrons inside it.
  Moderator(double p_scat, double scat_frac, double expand_coeff)
    : BasicMaterial(0, 0, p_scat, scat_frac, 0), expand_coeff_(expand_coeff) { };

  Moderator(const Moderator* f) : BasicMaterial(f) {
    expand_coeff_ = f->expand_coeff_;
  }

  virtual double scatter_prob(double speed) {
    double mult = expand_coeff_ / num_neutrons_;
    mult = std::max(0.5, mult);
    mult = std::min(2.0, mult);
    return p_scatter * mult;
  };

  virtual void Tick(double deltat, System* sys, std::vector<Neutron*> neutrons) {
    num_neutrons_ = neutrons.size();
  }

 private:
  int num_neutrons_;
  double expand_coeff_;

};

}

#endif
