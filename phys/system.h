
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <random>
#include <vector>

#include "phys/material.h"
#include "phys/geometry.h"
#include "phys/neutron.h"

namespace phys {

class System {

 public:
  System(Geometry geom) : geom_(geom) { };

  void Tick(double deltat) {
    auto it = neutrons_.begin();
    auto end = neutrons_.end();
    while (it != end) {
      Material m = geom_.MatFor(it->x(), it->y());

      Rxn rxn = SelectRxn(*it, m, deltat);
      if (rxn == SCATTER) {
        it->set_vxy(m.new_vx(it->vx()), m.new_vy(it->vy()));
        it->Move(deltat);
        ++it;
      } else if (rxn == ABSORB){
        it = neutrons_.erase(it);
      } else if (rxn == FISSION) {
        int yield = m.fiss_yield();
        for (int i = 0; i < yield; ++i) {
          double x = it->x();
          double y = it->y();
          double vx = FissV();
          double vy = FissV();
          neutrons_.push_back(Neutron(x, y, vx, vy));
        }
        it = neutrons_.erase(it);
      } else {
        it->Move(deltat);
        ++it;
      }
    }
  };

  const std::vector<Neutron> neutrons() {
    return neutrons_;
  };

 private:
   enum Rxn {NOTHING, FISSION, ABSORB, SCATTER};

   double FissV() {
     return poisson1_(rand_gen_);
   };

  Rxn SelectRxn(const Neutron& neut, Material& m, double deltat) {
    double speed = neut.speed();
    double distance = speed * deltat;
    double p_absorb = m.absorb_prob(speed) * distance;
    double p_fiss = m.fiss_prob(speed) * distance;
    double p_scatter = m.scatter_prob(speed) * distance;
    double p_nothing = 1 - (p_scatter + p_fiss + p_absorb);

    double rn = uniform01_(rand_gen_);
    if (rn <= p_nothing) {
      return NOTHING;
    } else if (rn <= p_nothing + p_scatter) {
      return SCATTER;
    } else if (rn <= p_nothing + p_scatter + p_absorb) {
      return ABSORB;
    } else {
      return FISSION;
    }
  };

  Geometry geom_;
  std::vector<Neutron> neutrons_;

  std::mt19937_64 rand_gen_;
  std::uniform_real_distribution<> uniform01_;
  std::poisson_distribution<> poisson1_;
};

} // namespace phys

#endif
