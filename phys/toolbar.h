#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <math.h>
#include <map>

#include "phys/object.h"

namespace phys {

class Toolbar : public Object {
 public:
  Toolbar() : ycurr_(20) {
    isToolbar_ = true;
  };

  Toolbar(const Toolbar* t) : Object(t) { };
  
  virtual Toolbar* clone() { return new Toolbar(&(*this)); };

  virtual double absorb_prob(double speed) { return 1; };

  Object::Rect* PlaceItem(Object::Rect* r, int pad_h = 20) {
    int dx = (r_.w - r->w) / 2;
    r->x = r_.x + dx;
    r->y = ycurr_;
    ycurr_ += r->h + pad_h;
    return r;
  }
  
 private:
  int ycurr_;
};

} // namespace phys

#endif
