
#ifndef NEUTRON_H_
#define NEUTRON_H_

#include <math.h>

namespace phys {

class Neutron {
 public:
  Neutron(double x, double y, double vx, double vy) : x_(x), y_(y), vx_(vx),
    vy_(vy) {
    speed_ = std::pow(vx_ * vx_ + vy_ * vy_, -.5);
  };

  void Move(double deltat) {
    x_ += deltat * vx_;
    y_ += deltat * vy_;
  };

  void set_vxy(double vx, double vy) {
    vx_ = vx;
    vy_ = vy;
    speed_ = std::pow(vx_ * vx_ + vy_ * vy_, -.5);
  };

  double x() const {
    return x_;
  }

  double y() const {
    return y_;
  }

  double vx() const {
    return vx_;
  }

  double vy() const {
    return vy_;
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
