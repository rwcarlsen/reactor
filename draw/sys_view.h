
#ifndef DRAW_NEUTRONS_H_
#define DRAW_NEUTRONS_H_

#include <sstream>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "sdl/renderer.h"
#include "sdl/color.h"
#include "sdl/texture.h"
#include "sdl/surface.h"
#include "sdl/font.h"

#include "phys/system.h"
#include "phys/object.h"

namespace draw {

static const char* kFontPath = "/usr/share/fonts/TTF/DejaVuSerif-Bold.ttf";

class SysView {
 public:
  SysView(const phys::System* sys, sdl::Renderer* ren) : sys_(sys), ren_(ren),
    font_(kFontPath) {
    bg_color_ = sdl::Color::black();
    neut_color_ = sdl::Color::red();
    font_color_ = sdl::Color::yellow();
  };

  void Render(double fps) {
    ren_->set_draw_color(bg_color_);
    ren_->Clear();

    DrawGeometry();
    DrawNeutrons();
    DrawInfo(fps);

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

  void DrawInfo(double fps) {
    std::stringstream ss;
    ss << "FPS: " << (int)fps;
    auto surf = font_.RenderBlended(ss.str().c_str(), font_color_);
    sdl::Texture tex(*ren_, *surf.get());
    tex.ApplyFull(10, 10);

    std::stringstream ss2;
    ss2 << "Neutrons: " << sys_->neutrons().size();
    auto surf2 = font_.RenderBlended(ss2.str().c_str(), font_color_);
    sdl::Texture tex2(*ren_, *surf2.get());
    tex2.ApplyFull(10, 30);
  }

  sdl::Color bg_color_;
  sdl::Color neut_color_;
  sdl::Color font_color_;

  const phys::System* sys_;
  sdl::Renderer* ren_;
  sdl::Font font_;
};

} // namespace draw

#endif
