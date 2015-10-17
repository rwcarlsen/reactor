#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <math.h>
#include <map>

#include "phys/object.h"




namespace phys {

  class Toolbar : public Object {

  public:
    Toolbar() { isToolbar_ = true;};

    Toolbar(Object::Rect bounds) {
      isToolbar_ = true;
      Init(bounds, sdl::Color::gray());
    };

    virtual Toolbar* clone() { return new Toolbar(*this); };

    virtual sdl::Surface* surface() {

      surf_->FillRect(NULL, sdl::Color::gray() );

      return surf_;
    };

    virtual double absorb_prob(double speed) {
      return 1;
    };

  };

} // namespace phys

#endif
