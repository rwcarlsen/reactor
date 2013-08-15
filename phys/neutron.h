
#ifndef NEUTRON_H_
#define NEUTRON_H_

namespace phys {

class Neutron {
 public:
  Neutron(double x, double y, double vx, double vy) : x_(x), y_(y), vx_(vx),
    vy_(vy) { };

  Move(double deltat) {
    x_ += deltat * vx_;
    y_ += deltat * vy_;
  };

  void SpeedMult(double frac) {
    vx_ *= frac;
    vy_ *= frac;
  };

  int xpos() {
    return x_;
  }

  int ypos() {
    return y_;
  }

 private:
  double x_, y_;
  double vx_, vy_;
}

} // namespace phys

#endif
