
#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <unordered_map>
#include <vector>

#include "sdl/surface.h"
#include "sdl/color.h"
#include "phys/neutron.h"

namespace phys {

class System;

class Object {
 public:
  typedef SDL_Rect Rect;

 Object() : detector_(false), isToolbar_(false), surf_(nullptr) { };

 Object(Rect bounds, sdl::Color c = sdl::Color::white()) : detector_(false), isToolbar_(false) {
    Init(bounds, c);
  };

  Object(const Object* obj) {
    detector_ = obj->detector_;
    isToolbar_ = obj->isToolbar_;
    r_ = obj->r_;
    color_ = obj->color_;
    surf_ = new sdl::Surface(r_.w, r_.h);
    surf_->FillRect(NULL, color_);
  };

  virtual Object* clone() { return new Object(&(*this)); };

  virtual void Resize(int x, int y, int dx, int dy) {
    Object::Rect rnew = r_;
    rnew.w += dx;
    rnew.h += dy;
    rnew.w = std::max(rnew.w, 10);
    rnew.h = std::max(rnew.h, 10);
    Init(rnew, color_);
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

  bool detector() { return detector_; };

  bool isToolbar() { return isToolbar_; };

  virtual bool OnClick(int x, int y) {
    return false; // derived classes return true if no burst wanted
  }

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

  virtual bool React(System* sys, Rxn rx, double x, double y) {return true;};

  virtual Neutron::V scat_v(Neutron::V v, double speed) {
    return v;
  };

  virtual int fiss_yield() {
    return 0;
  };

  virtual void Tick(double deltat, System* sys, std::vector<Neutron*> neutrons) { };

 protected:
  bool detector_;
  bool isToolbar_;
  Rect r_;
  sdl::Surface* surf_;
  sdl::Color color_;
};

} // namespace phys

#endif
