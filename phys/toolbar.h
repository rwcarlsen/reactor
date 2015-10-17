#ifndef TOOLBAR_H_
#define TOOLBAR_H_

#include <math.h>
#include <map>

#include "phys/object.h"



namespace phys {

  class Toolbar : public Object {

  public:
    Toolbar() { };

    virtual sdl::Surface* surface() {

      surf_->FillRect(NULL, sdl::Color::white() );

      return surf_;
    };

  };

} // namespace phys

#endif
