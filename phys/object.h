
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "sdl/surface.h"
#include "sdl/color.h"
#include "phys/neutron.h"

namespace phys {

class Object {
 public:
  typedef SDL_Rect Rect;

  Object() : surf_(nullptr) { };
  
  Object(Rect bounds, sdl::Color c = sdl::Color::white()) {
    Init(bounds, c);
  };

  Object(const Object* obj) {
    r_ = obj->r_;
    color_ = obj->color_;
    surf_ = new sdl::Surface(r_.w, r_.h);
    surf_->FillRect(NULL, color_);
  };

  void Init(Rect bounds, sdl::Color c = sdl::Color::white()) {
    r_ = bounds;
    color_ = c;
    surf_ = new sdl::Surface(bounds.w, bounds.h);
    surf_->FillRect(NULL, c);
  };

  bool Contains(int x, int y) const {
    return (x >= r_.x) && (x < r_.x + r_.w) &&
           (y >= r_.y) && (y < r_.y + r_.h);
  };

  Rect rect() {
    return r_;
  }

  virtual sdl::Surface* surface() {
    return surf_;
  }

  void Shift(int dx, int dy) {
    r_.x += dx;
    r_.y += dy;
  };

  //////////// material properties ////////////
  virtual double absorb_prob(double speed) {
    return 0;
  };

  virtual double fiss_prob(double speed) {
    return 0;
  };

  virtual double scatter_prob(double speed) {
    return 0;
  };

  virtual bool React(Rxn rx, double x, double y) {return true;};

  virtual Neutron::V scat_v(Neutron::V v, double speed) {
    return v;
  };

  virtual int fiss_yield() {
    return 0;
  };

  virtual void tick_info(double deltat, std::vector<Neutron*> neutrons) { };

 protected:
  Rect r_;
  sdl::Surface* surf_;
  sdl::Color color_;
};

} // namespace phys

#endif
