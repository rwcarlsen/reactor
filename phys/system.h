
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <random>
#include <vector>

#include "phys/material.h"
#include "phys/object.h"
#include "phys/neutron.h"

namespace phys {

class System {
 public:
  System(int w, int h)
    : width_(w),
      height_(h),
    blank_(new Material(), Object::Rect {0, 0, 1, 1}) { };

  ~System() {
    delete blank_.material();
  };

  void AddObject(Object o) {
    objs_.push_back(o);
  };

  void AddNeutrons(Neutron::Pop ns) {
    neutrons_.insert(neutrons_.end(), ns.begin(), ns.end());
  };

  void Tick(double deltat) {
    int i = 0;
    while (i < neutrons_.size()) {
      Neutron* n = &neutrons_[i];
      Object* obj = ObjectFor(n->x(), n->y());
      Material* m = obj->material();

      // remove out of bounds neutrons
      if (n->x() < 0 || n->y() < 0 || n->x() > width_ || n->y() > height_) {
        neutrons_[i] = neutrons_[neutrons_.size() - 1];
        neutrons_.pop_back();
        continue;
      }

      Rxn rxn = SelectRxn(n, m, deltat);
      if (rxn == SCATTER) {
        n->set_v(m->scat_v(n->v(), n->speed()));
        n->Move(deltat);
        ++i;
      } else if (rxn == ABSORB) {
        neutrons_[i] = neutrons_[neutrons_.size() - 1];
        neutrons_.pop_back();
      } else if (rxn == FISSION) {
        int yield = m->fiss_yield();
        double x = n->x();
        double y = n->y();
        for (int j = 0; j < yield; ++j) {
          V v = FissV();
          neutrons_.push_back(Neutron(x, y, v.x, v.y));
        }

        neutrons_[i] = neutrons_[neutrons_.size() - 1];
        neutrons_.pop_back();
      } else {
        n->Move(deltat);
        ++i;
      }
    }
  };

  Object* ObjectFor(int x, int y) {
    for (int i = objs_.size() - 1; i >= 0; --i) {
      if (objs_[i].Contains(x, y)) {
        return &objs_[i];
      }
    }
    return &blank_;
  }

  const Neutron::Pop neutrons() const {
    return neutrons_;
  };

  const std::vector<Object>& objects() const {
    return objs_;
  };

 private:
  enum Rxn {NOTHING, FISSION, ABSORB, SCATTER};
  struct V {
    double x;
    double y;
  };

  Rxn SelectRxn(const Neutron* neut, Material* m, double deltat) {
    double speed = neut->speed();
    double distance = speed * deltat;
    double p_absorb = m->absorb_prob(speed) * distance;
    double p_fiss = m->fiss_prob(speed) * distance;
    double p_scatter = m->scatter_prob(speed) * distance;
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

  V FissV() {
    V v;
    double theta = uniform01_(rand_gen_) * 2 * kPi;
    double speed = 30 * poisson1_(rand_gen_) + 5;
    v.x = std::cos(theta) * speed;
    v.y = std::sin(theta) * speed;
    return v;
  };

  std::vector<Object> objs_;
  Object blank_;
  Neutron::Pop neutrons_;

  int width_;
  int height_;

  std::minstd_rand rand_gen_;
  std::uniform_real_distribution<> uniform01_;
  std::poisson_distribution<> poisson1_;
};

} // namespace phys

#endif
