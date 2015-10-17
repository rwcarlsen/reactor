#ifndef DETECTOR_H_
#define DETECTOR_H_

#include <math.h>
#include <map>

#include "phys/object.h"
#include "draw/sys_view.h"

namespace phys {

class Detector : public Object {
 public:
  Detector() : prev_n_(0), curr_n_(0), deltat_(1) {
    detector_ = true;
    draw::LoadFont(&font_);
  };

  Detector(const Detector* f) : Object(f) {
    detector_ = true;
    draw::LoadFont(&font_);
    prev_n_ = f->prev_n_;
    curr_n_ = f->curr_n_;
    deltat_ = f->deltat_;
  }

  virtual sdl::Surface* surface() {
    std::string n_inside = draw::FixedWidthInt((int)curr_n_, 4);
    std::stringstream ss;
    ss << n_inside;

    surf_->FillRect(NULL, sdl::Color::yellow(128));

    auto fontsurf = font_.RenderBlended(ss.str().c_str(), sdl::Color::black());

    int x = (surf_->width() - fontsurf->width()) / 2;
    int y = (surf_->height() - fontsurf->height()) / 2;

    SDL_Rect r = {x, y, fontsurf->width(), fontsurf->height()};
    surf_->Blit(fontsurf.get(), &r);

    return surf_;
  }

  virtual void tick_info(double deltat, std::vector<Neutron*> neutrons, double neutron_weight) {
    prev_n_ = curr_n_;
    curr_n_ = neutrons.size() * neutron_weight;
    deltat_ = deltat;
  }

 private:
  sdl::Font font_;
  int prev_n_;
  int curr_n_;
  double deltat_;
};

}

#endif
