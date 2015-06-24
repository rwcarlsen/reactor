
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <random>
#include <vector>
#include <list>

#include "phys/object.h"
#include "phys/neutron.h"

#define SHOW(X) \
  std::cout << __FILE__ << ":" << __LINE__ << ": "#X" = " << X << "\n"

namespace phys {

class System {
 public:
  System(int w, int h)
    : width_(w),
      height_(h),
      prevn_(0),
      normal1_(Neutron::kNomSpeed, Neutron::kNomSpeed / 8.0),
    blank_(Object::Rect {0, 0, 1, 1}) { };

  ~System() { };

  void AddObject(Object* o) {
    objs_.push_back(o);
  };

  void AddNeutrons(Neutron::Pop ns) {
    neutrons_.insert(neutrons_.end(), ns.begin(), ns.end());
  };

  double period() const {
    return period_;
  }

  void Tick(double deltat) {
    int i = 0;
    while (i < neutrons_.size()) {
      Neutron* n = &neutrons_[i];
      Object* obj = ObjectFor(n->x(), n->y());

      // remove out of bounds neutrons
      if (n->x() < 0 || n->y() < 0 || n->x() > width_ || n->y() > height_) {
        neutrons_[i] = neutrons_[neutrons_.size() - 1];
        neutrons_.pop_back();
        continue;
      }

      Rxn rxn = SelectRxn(n, obj, deltat);
      if (!obj->React(rxn, n->x(), n->y())) {
        n->Move(deltat);
        ++i;
        continue;
      }

      if (rxn == RxScatter) {
        n->set_v(obj->scat_v(n->v(), n->speed()));
        n->Move(deltat);
        ++i;
      } else if (rxn == RxAbsorb) {
        neutrons_[i] = neutrons_[neutrons_.size() - 1];
        neutrons_.pop_back();
      } else if (rxn == RxFission) {
        int yield = obj->fiss_yield();
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

    cumdt_ += deltat;
    double window_sec = 2;
    if (cumdt_ >= window_sec) {
      double currn = neutrons_.size();
      if (currn > 100 && prevn_ > 100 && currn != prevn_) {
        period_ = cumdt_ / std::log(currn / prevn_);
      }
      cumdt_ = 0;
      prevn_ = neutrons_.size();
    }
  };

  void MoveTop(Object* obj) {
    for (int i = 0; i < objs_.size(); ++i) {
      if (objs_[i] == obj) {
        objs_.erase(objs_.begin() + i);
        objs_.push_back(obj);
        break;
      }
    }
  }

  Object* ObjectFor(int x, int y) {
    for (int i = objs_.size() - 1; i >= 0; --i) {
      if (objs_[i]->Contains(x, y)) {
        return objs_[i];
      }
    }
    return &blank_;
  }

  const Neutron::Pop neutrons() const {
    return neutrons_;
  };

  const std::vector<Object*>& objects() const {
    return objs_;
  };

 private:
  struct V {
    double x;
    double y;
  };

  Rxn SelectRxn(const Neutron* neut, Object* obj, double deltat) {
    double speed = neut->speed();
    double distance = speed * deltat;
    double p_absorb = obj->absorb_prob(speed) * distance;
    double p_fiss = obj->fiss_prob(speed) * distance;
    double p_scatter = obj->scatter_prob(speed) * distance;
    double p_nothing = 1 - (p_scatter + p_fiss + p_absorb);

    double rn = uniform01_(rand_gen_);
    if (rn <= p_nothing) {
      return RxNothing;
    } else if (rn <= p_nothing + p_scatter) {
      return RxScatter;
    } else if (rn <= p_nothing + p_scatter + p_absorb) {
      return RxAbsorb;
    } else {
      return RxFission;
    }
  };

  V FissV() {
    V v;
    double theta = uniform01_(rand_gen_) * 2 * kPi;
    double speed = normal1_(rand_gen_);
    v.x = std::cos(theta) * speed;
    v.y = std::sin(theta) * speed;
    return v;
  };

  std::vector<Object*> objs_;
  Object blank_;
  Neutron::Pop neutrons_;
  double prevn_;
  double cumdt_;
  double period_;

  int width_;
  int height_;

  std::minstd_rand rand_gen_;
  std::uniform_real_distribution<> uniform01_;
  std::normal_distribution<> normal1_;
};

} // namespace phys

#endif
