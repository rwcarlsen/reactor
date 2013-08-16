
#ifndef MATERIAL_H_
#define MATERIAL_H_

namespace phys {

// all probabilities given per unit length
class Material {
 public:
   virtual double absorb_prob(double speed) {
     return 0;
   };

   virtual double fiss_prob(double speed) {
     return 0;
   };

   virtual double scatter_prob(double speed) {
     return 0;
   };

   virtual double new_vx(double vx) {
     return vx;
   };

   virtual double new_vy(double vy) {
     return vy;
   };

   virtual int fiss_yield() {
     return 0;
   };
};

} // namespace phys

#endif
