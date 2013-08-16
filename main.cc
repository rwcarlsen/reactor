
#include <iostream>

#include "SDL2/SDL.h"

#include "sdl/error.h"
#include "sdl/window.h"
#include "sdl/renderer.h"
#include "sdl/texture.h"
#include "sdl/sdl_init.h"
#include "sdl/color.h"
#include "sdl/timer.h"

#include "phys/system.h"
#include "phys/basic_material.h"

using sdl::Color;

int main(int argc, char** argv) {
  try {
    // create window
    sdl::SDLinit init(SDL_INIT_EVERYTHING);

    int w = 640;
    int h = 480;
    sdl::Window win("Reactor", w, h, 0);
    win.Maximize();
    win.Show();
    sdl::Renderer ren(win, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // create geometry
    phys::BasicMaterial m{0, 0, .1, .99, 0};
    phys::Geometry::Rect r{0, 0, 640, 480};
    phys::Geometry geom;
    geom.AddMaterial(&m, r);

    // create neutron system
    phys::System sys(geom);
    phys::Neutron::Pop ns;
    for (int i = 0; i < 30000; ++i) {
      ns.push_back(phys::Neutron(320, 240, .5, .5));
    }
    sys.AddNeutrons(ns);

    // set params
    double fps = 38;
    double pix_per_sec = 30;
    double deltat = pix_per_sec / fps;
    int neut_len = 2;

    sdl::Timer timer;
    timer.set_framerate(fps);

    // run simulation
    SDL_Event ev;
    timer.Start();
    bool done = false;
    while(!done) {
      while(SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
          done = true;
        }
      }

      sys.Tick(deltat);

      sdl::Surface surf(w, h);
      const phys::Neutron::Pop ns = sys.neutrons();
      for (auto it = ns.begin(); it != ns.end(); ++it) {
        SDL_Rect dst = {it->x(), it->y(), neut_len, neut_len};
        surf.FillRect(&dst, Color::red());
      }

      ren.Clear();
      sdl::Texture tex(ren, surf);
      tex.ApplyFull(0, 0);
      ren.Render();

      timer.Wait();
    }

    return 0;
  } catch (std::exception err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


