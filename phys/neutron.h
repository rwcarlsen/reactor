
#ifndef NEUTRON_H_
#define NEUTRON_H_

#include <math.h>

namespace phys {

class Neutron {
 public:
  typedef struct{double x; double y;} V;
  typedef std::vector<Neutron> Pop;

  Neutron(double x, double y, double vx, double vy) : x_(x), y_(y), vx_(vx),
    vy_(vy) {
    speed_ = std::pow(vx_ * vx_ + vy_ * vy_, 0.5);
  };

  void Move(double deltat) {
    x_ += deltat * vx_;
    y_ += deltat * vy_;
  };

  V v() const {
    V v = {vx_, vy_};
    return v;
  }

  void set_v(V v) {
    vx_ = v.x;
    vy_ = v.y;
    speed_ = std::pow(vx_ * vx_ + vy_ * vy_, 0.5);
  };

  double x() const {
    return x_;
  }

  double y() const {
    return y_;
  }

  double speed() const {
    return speed_;
  }

 private:
  double x_, y_;
  double vx_, vy_;
  double speed_;
};

} // namespace phys

#endif
