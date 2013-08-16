
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
    sdl::Window win(w, h, SDL_WINDOW_SHOWN);
    win.set_title("Hello World");
    win.Center();

    sdl::Renderer ren = win.renderer();

    // create geometry
    phys::BasicMaterial m{0, 0, .05, .99, 0};
    phys::Geometry::Rect r{0, 0, 640, 480};
    phys::Geometry geom;
    geom.AddMaterial(&m, r);
    geom.Build();

    // create neutron system
    phys::System sys(geom);
    phys::Neutron::Pop ns;
    ns.push_back(phys::Neutron(320, 240, .5, .5));
    sys.AddNeutrons(ns);

    // set params
    sdl::Timer timer;
    timer.set_framerate(10);
    double deltat = 0.2;

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

      ren.Clear();
      const phys::Neutron::Pop ns = sys.neutrons();
      for (int i = 0; i < ns.size(); ++i) {
        const phys::Neutron n = ns[i];
        SDL_Rect dst = {n.x(), n.y(), 5, 5};
        ren.FillRect(&dst, Color::red());
      }
      ren.Render();

      timer.Wait();

      // safety
      if (SDL_GetTicks() > 10000) {
        done = true;
      }
    }

    return 0;
  } catch (std::exception err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


