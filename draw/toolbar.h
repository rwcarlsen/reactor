#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "sdl/surface.h"
#include "sdl/color.h"
#include "sys/window.h"


namespace draw{

  class Toolbar {
  public:
    typdef SDL_Rect Rect;

    Toolbar() { };
    
    Toolbar(sdl::Window* win)
      {
	Rect r;
	int *w,*h;
        win.size(w,h);

	r.x = 0;
	r.y = 0;
	r.w = w*0.05;
	r.h = 0;

	Init(r);
      };

    void Init(Rect bounds, sdl::Color c = sdl::Color::white())
    {
      r_=bounds;
      color_ = c;
      surf_ = new sdl::Surface(bounds.w, bounds.h);
      surf_->FillRect(NULL, c);
    };
    
  protected:
    Rect r_;
    sdl::Surface* surf_;
    sdl::Color color_;
  }

} //namespace toolbar

#endif
