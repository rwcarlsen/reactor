
#ifndef DRAW_NEUTRONS_H_
#define DRAW_NEUTRONS_H_

#include "SDL2/SDL.h"

#include "sdl/renderer.h"
#include "sdl/color.h"
#include "sdl/texture.h"
#include "sdl/surface.h"

#include "phys/system.h"
#include "phys/object.h"

namespace draw {

class SysView {
 public:
  SysView(const phys::System* sys, sdl::Renderer* ren) : sys_(sys), ren_(ren) {
    bg_color_ = sdl::Color::black();
    neut_color_ = sdl::Color::red();
  };

  void Render() {
    ren_->set_draw_color(bg_color_);
    ren_->Clear();

    DrawGeometry();
    DrawNeutrons();

    ren_->Render();
  };

 private:
  void DrawGeometry() {
    const std::vector<phys::Object> objs = sys_->objects();
    for (int i = 0; i < objs.size(); ++i) {
      phys::Object obj = objs[i];
      sdl::Surface* surf = obj.surface();
      phys::Object::Rect r = obj.rect();
      sdl::Texture tex(*ren_, *surf);
      tex.ApplyFull(r.x, r.y);
    }
  };

  void DrawNeutrons() {
    const phys::Neutron::Pop ns = sys_->neutrons();
    SDL_Point points[ns.size()];
    int i = 0;
    for (auto it = ns.begin(); it != ns.end(); ++it) {
      i++;
      points[i].x = it->x();
      points[i].y = it->y();
    }

    ren_->set_draw_color(neut_color_);
    ren_->DrawPoints(points, ns.size());
  };

  sdl::Color bg_color_;
  sdl::Color neut_color_;

  const phys::System* sys_;
  sdl::Renderer* ren_;
};

} // namespace draw

#endif
