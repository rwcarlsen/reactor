
#ifndef MATERIAL_H_
#define MATERIAL_H_

namespace phys {

class Material {
 public:
   double absorb_prob(double speed) = 0;
   double fiss_prob(double speed) = 0;
   double fiss_yield() = 0;
   double scatter_prob(double speed) = 0;
   double scatter_vx(double x, double y, double vx) = 0;
   double scatter_vy(double x, double y, double vy) = 0;
}

} // namespace phys

#endif
