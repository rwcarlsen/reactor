
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

#include "draw/sys_view.h"

using sdl::Color;

int main(int argc, char** argv) {
  try {
    sdl::SDLinit init(SDL_INIT_EVERYTHING);

    // create window and renderer
    int w = 850;
    int h = 650;
    sdl::Window win("Reactor", w, h, 0);
    win.Center();
    win.Show();
    sdl::Renderer ren(win, SDL_RENDERER_ACCELERATED);

    // create material geometry
    phys::BasicMaterial m{.002, 0, .1, .99, 0};
    phys::Object::Rect r{0, 0, w, h};
    phys::Object obj(&m, r);

    // create system and a view for drawing it
    phys::System sys;
    sys.AddObject(obj);
    draw::SysView view(&sys, &ren);

    // start up the main loop
    SDL_Event ev;
    sdl::Timer timer;
    timer.Start();
    bool done = false;
    while(!done) {
      // process events
      while(SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
          done = true;
        }
      }
      
      // calculate timing
      double dt = (double)timer.Mark();
      int fps = 1000 / dt;
      std::cout << fps << " fps, N = " << sys.neutrons().size() << "\n";
      sys.Tick(dt / 1000);

      // adjust neutron population
      if (fps > 35) {
        phys::Neutron::Pop ns;
        for (int i = 0; i < 5000; ++i) {
          ns.push_back(phys::Neutron(w / 2, h / 2, 15, 15));
        }
        sys.AddNeutrons(ns);
      }

      // draw everything to the screen
      view.Render();
    }

    return 0;
  } catch (std::exception err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}


