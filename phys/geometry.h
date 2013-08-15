
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

namespace phys {

class Geometry {
 public:
   double absorb_prob(double x, double y, double speed) = 0;
   double fiss_prob(double x, double y, double speed) = 0;
   double fiss_yield(double x, double y) = 0;
   double scatter_prob(double x, double y, double speed) = 0;
   double scatter_vx(double x, double y, double vx) = 0;
   double scatter_vy(double x, double y, double vy) = 0;
}

} // namespace phys

#endif
