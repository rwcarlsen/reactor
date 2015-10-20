
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <random>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include "phys/object.h"
#include "phys/neutron.h"
#include "phys/toolbar.h"

#define SHOW(X) \
  std::cout << __FILE__ << ":" << __LINE__ << ": "#X" = " << X << "\n"

namespace phys {

class System {
 public:
  System(int w, int h)
    : width_(w),
      height_(h),
      neutron_weight_(1),
      prevpow_(0),
      normal1_(Neutron::kNomSpeed, Neutron::kNomSpeed / 8.0),
      blank_(Object::Rect {0, 0, 1, 1}) {
    //fraction of window width for toolbar
    int tb_w = 150;
    toolbar_.Init(Object::Rect {width_ - tb_w,0,tb_w,height_}, sdl::Color::gray());
    AddObject(&toolbar_);
  };

  ~System() { };

  bool InToolbar(Object* o) {
    SDL_Rect a = o->rect();
    SDL_Rect b = toolbar_.rect();
    return SDL_HasIntersection(&a, &b);
  };

  Toolbar& toolbar() {return toolbar_; };

  void AddObject(Object* o) {
    objs_.push_back(o);
  };

  void RemoveObject(Object *o)
  {
    std::vector<Object*>::iterator it;
    for(unsigned int i=0; i < objs_.size(); i++)
      {
	if ( objs_[i] == o )
	  {
	    it = objs_.erase(objs_.begin()+i);
	  }
      }
  };

  void AddNeutrons(Neutron::Pop ns) {
    neutrons_.insert(neutrons_.end(), ns.begin(), ns.end());
  };

  double period() const {
    return period_;
  }

  void Tick(double deltat) {
    // roulette off neutrons if we have too many
    double dtmax = 0.03;
    if (deltat > 1.1*dtmax) {
      double pkill = (deltat - dtmax) / deltat;
      int nkill = 0;
      int i = 0;
      while (i < neutrons_.size()) {
        double r = uniform01_(rand_gen_);
        if (r < pkill) {
          nkill++;
          neutrons_[i] = neutrons_[neutrons_.size() - 1];
          neutrons_.pop_back();
        } else {
          i++;
        }
      }

      neutron_weight_ *= (double)(neutrons_.size() + nkill) / (double)neutrons_.size();
    } else if (deltat < 0.9*dtmax && neutron_weight_ > 1) {
      double pdup = (dtmax - deltat) / dtmax;
      int norig = neutrons_.size();
      for (int i = 0; i < norig; i++) {
        double r = uniform01_(rand_gen_);
        if (r < pdup) {
          neutrons_.push_back(phys::Neutron(neutrons_[i]));
        }
      }
      neutron_weight_ *= (double)norig / (double)neutrons_.size();
      neutron_weight_ = std::max(1.0, neutron_weight_);
    }

    int i = 0;
    std::map<Object*, std::vector<Neutron*> > neuts;
    while (i < neutrons_.size()) {
      Neutron* n = &neutrons_[i];

      // remove out of bounds neutrons
      if (n->x() < 0 || n->y() < 0 || n->x() > width_ || n->y() > height_) {
        neutrons_[i] = neutrons_[neutrons_.size() - 1];
        neutrons_.pop_back();
        continue;
      }

      Object* obj = MaterialFor(n->x(), n->y());
      neuts[obj].push_back(n);

      std::vector<Object*> dets = DetectorsFor(n->x(), n->y());
      for (auto det : dets) {
        neuts[det].push_back(n);
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
      double currpow = power();
      if (currpow > 100 && prevpow_ > 100 && currpow != prevpow_) {
        period_ = cumdt_ / std::log(currpow / prevpow_);
      }
      cumdt_ = 0;
      prevpow_ = currpow;
    }

    for (int i = 0; i < objs_.size(); i++) {
      objs_[i]->tick_info(deltat, neuts[objs_[i]], neutron_weight_);
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

  Object* MaterialFor(int x, int y) {
    for (int i = objs_.size() - 1; i >= 0; --i) {
      if (objs_[i]->Contains(x, y) && !objs_[i]->detector()) {
        return objs_[i];
      }
    }
    return &blank_;
  }

  std::vector<Object*> DetectorsFor(int x, int y) {
    std::vector<Object*> dets;
    for (int i = objs_.size() - 1; i >= 0; --i) {
      if (objs_[i]->Contains(x, y) && objs_[i]->detector()) {
        dets.push_back(objs_[i]);
      }
    }
    return dets;
  }

  const Neutron::Pop neutrons() const {
    return neutrons_;
  };

  double neutron_weight() const { return neutron_weight_; }

  double power() const { return neutron_weight_ * neutrons_.size(); }

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
  Toolbar toolbar_;
  Neutron::Pop neutrons_;
  double prevpow_;
  double cumdt_;
  double period_;
  double neutron_weight_;

  int width_;
  int height_;

  std::minstd_rand rand_gen_;
  std::uniform_real_distribution<> uniform01_;
  std::normal_distribution<> normal1_;
};

} // namespace phys

#endif
