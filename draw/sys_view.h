
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

  font->Load(fontpath);
}

class SysView {
 public:
  SysView(const phys::System* sys, sdl::Renderer* ren)
      : pow_(0),
        nneutrons_(0),
        period_(0),
        fps_(0),
        cumdt_(0),
        sys_(sys),
        ren_(ren) {
    LoadFont(&font_);
    bg_color_ = sdl::Color::black();
    slow_neut_color_ = sdl::Color::red();
    fast_neut_color_ = sdl::Color::lilac();
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
    SDL_Point* fast_points = new SDL_Point[ns.size()];
    SDL_Point* slow_points = new SDL_Point[ns.size()];

    int nfast = 0;
    int nslow = 0;
    for (auto it = ns.begin(); it != ns.end(); ++it) {
      if (it->speed() > 0.5 * phys::Neutron::kNomSpeed) {
        fast_points[nfast].x = it->x();
        fast_points[nfast].y = it->y();
        nfast++;
      } else {
        slow_points[nslow].x = it->x();
        slow_points[nslow].y = it->y();
        nslow++;
      }
    }

    ren_->set_draw_color(fast_neut_color_);
    ren_->DrawPoints(fast_points, nfast);
    ren_->set_draw_color(slow_neut_color_);
    ren_->DrawPoints(slow_points, nslow);
    delete[] fast_points;
    delete[] slow_points;
  };

  void DrawInfo(double fps) {
    double deltat_sec = 1.0 / fps;
    cumdt_ += deltat_sec;

    // this prevents the info from changing too quickly to read on screen
    if (cumdt_ > 1) {
      cumdt_ = 0;
      pow_ = sys_->power();
      nneutrons_ = sys_->neutrons().size();
      period_ = sys_->period();
      fps_ = fps;
    }

    std::string sfps = FixedWidthInt(fps_, 4);
    std::stringstream ss;
    ss << "FPS: " << sfps;
    auto surf = font_.RenderBlended(ss.str().c_str(), font_color_);
    sdl::Texture tex(*ren_, *surf.get());
    tex.ApplyFull(10, 10);

    std::stringstream ss4;
    ss4 << "Power: " << std::scientific << pow_;
    auto surf4 = font_.RenderBlended(ss4.str().c_str(), font_color_);
    sdl::Texture tex4(*ren_, *surf4.get());
    tex4.ApplyFull(10, 30);

    std::stringstream ss2;
    ss2 << "Neutrons: " << std::scientific << (double)nneutrons_;
    auto surf2 = font_.RenderBlended(ss2.str().c_str(), font_color_);
    sdl::Texture tex2(*ren_, *surf2.get());
    tex2.ApplyFull(10, 50);

    std::stringstream ss3;
    ss3 << "Period: " << period_;
    auto surf3 = font_.RenderBlended(ss3.str().c_str(), font_color_);
    sdl::Texture tex3(*ren_, *surf3.get());
    tex3.ApplyFull(10, 70);
  }

  double cumdt_;
  double pow_;
  int nneutrons_;
  double period_;
  int fps_;

  sdl::Color bg_color_;
  sdl::Color fast_neut_color_;
  sdl::Color slow_neut_color_;
  sdl::Color font_color_;

  const phys::System* sys_;
  sdl::Renderer* ren_;
  sdl::Font font_;
};

} // namespace draw

#endif
