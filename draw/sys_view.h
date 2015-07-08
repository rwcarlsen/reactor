
#ifndef DRAW_NEUTRONS_H_
#define DRAW_NEUTRONS_H_

#include <sstream>
#include <fstream>
#include <iomanip>
#include <sys/stat.h>

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

std::string FixedWidthInt(int v, int len) {
  std::string fixed;
  std::stringstream ss;
  std::stringstream ssf;
  ss << v;
  if (ss.str().size() < len) {
    for (int i = 0; i < len - ss.str().size(); ++i) {
      ssf << "0";
    }
  }
  ssf << ss.str();
  return ssf.str();
}

static const char* kFontPath = "/usr/share/fonts/TTF/Ubuntu-R.ttf";

bool fileExists(std::string fname) {
  std::ifstream infile(fname);
  return infile.good();
}

std::string env(std::string var) {
  char* s = getenv(var.c_str());
  if (s == NULL) {
    return "";
  }
  return s;
}

void LoadFont(sdl::Font* font) {
  std::string fontpath(env("REACTOR_FONT"));
  if (fontpath == "") {
    fontpath = "/usr/share/reactor/FreeMono.ttf";
    if (!fileExists(fontpath)) {
      fontpath = std::string(env("HOME")) + ".local/share/reactor/FreeMono.ttf";
      if (!fileExists(fontpath)) {
        fontpath = "./FreeMono.ttf";
      }
    }
  }

  font->Load(fontpath, 32);
}

class SysView {
 public:
  SysView(const phys::System* sys, sdl::Renderer* ren) : sys_(sys), ren_(ren) {
    LoadFont(&font_);
    bg_color_ = sdl::Color::black();
    neut_color_ = sdl::Color::red();
    font_color_ = sdl::Color::yellow();
  };

  void Render(double fps) {
    ren_->set_draw_color(bg_color_);
    ren_->Clear();

    DrawMaterials();
    DrawNeutrons();
    DrawDetectors();
    DrawInfo(fps);

    ren_->Render();
  };

 private:
  void DrawMaterials() {
    const std::vector<phys::Object*> objs = sys_->objects();
    for (int i = 0; i < objs.size(); ++i) {
      phys::Object* obj = objs[i];
      if (obj->detector()) {
        continue;
      }
      sdl::Surface* surf = obj->surface();
      phys::Object::Rect r = obj->rect();
      sdl::Texture tex(*ren_, *surf);
      tex.ApplyFull(r.x, r.y);
    }
  };

  void DrawDetectors() {
    const std::vector<phys::Object*> objs = sys_->objects();
    for (int i = 0; i < objs.size(); ++i) {
      phys::Object* obj = objs[i];
      if (!obj->detector()) {
        continue;
      }
      sdl::Surface* surf = obj->surface();
      phys::Object::Rect r = obj->rect();
      sdl::Texture tex(*ren_, *surf);
      tex.ApplyFull(r.x, r.y);
    }
  };

  void DrawNeutrons() {
    const phys::Neutron::Pop ns = sys_->neutrons();
    SDL_Point* points = new SDL_Point[ns.size()];
    int i = 0;
    for (auto it = ns.begin(); it != ns.end(); ++it) {
      points[i].x = it->x();
      points[i].y = it->y();
      i++;
    }

    ren_->set_draw_color(neut_color_);
    ren_->DrawPoints(points, ns.size());
    delete[] points;
  };

  void DrawInfo(double fps) {
    std::string sfps = FixedWidthInt((int)fps, 4);
    std::stringstream ss;
    ss << "FPS: " << sfps;
    auto surf = font_.RenderBlended(ss.str().c_str(), font_color_);
    sdl::Texture tex(*ren_, *surf.get());
    tex.ApplyFull(10, 10);

    std::string pow = FixedWidthInt(sys_->power(), 7);
    std::stringstream ss4;
    ss4 << "Power: " << pow;
    auto surf4 = font_.RenderBlended(ss4.str().c_str(), font_color_);
    sdl::Texture tex4(*ren_, *surf4.get());
    tex4.ApplyFull(10, 50);

    std::string nn = FixedWidthInt(sys_->neutrons().size(), 7);
    std::stringstream ss2;
    ss2 << "Neutrons: " << nn;
    auto surf2 = font_.RenderBlended(ss2.str().c_str(), font_color_);
    sdl::Texture tex2(*ren_, *surf2.get());
    tex2.ApplyFull(10, 90);

    std::stringstream ss3;
    ss3 << "Period: " << sys_->period();
    auto surf3 = font_.RenderBlended(ss3.str().c_str(), font_color_);
    sdl::Texture tex3(*ren_, *surf3.get());
    tex3.ApplyFull(10, 130);
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
