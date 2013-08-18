
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
    phys::BasicMaterial m1{0, 0, .1, 1, 0};
    phys::Object::Rect r1{w/2 - 40, h/2 - 40, 80, 80};
    phys::Object reflector(&m1, r1, sdl::Color::white());

    phys::BasicMaterial m2{.1, 0, 0, 1, 0};
    phys::Object::Rect r2{w/2 - 130, h/2 - 40, 80, 80};
    phys::Object absorber(&m2, r2, sdl::Color::blue());

    phys::BasicMaterial m3{0, 0, .1, .8, 0};
    phys::Object::Rect r3{w/2 + 50, h/2 - 40, 80, 80};
    phys::Object moderator(&m3, r3, sdl::Color::green());

    // create system and a view for drawing it
    phys::System sys(w, h);
    sys.AddObject(reflector);
    sys.AddObject(absorber);
    sys.AddObject(moderator);
    draw::SysView view(&sys, &ren);

    // start up the main loop
    SDL_Event ev;
    sdl::Timer timer;
    timer.Start();
    bool done = false;
    bool dragging = false;
    phys::Object* dragged = nullptr;
    while(!done) {
      // process events
      while(SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
          done = true;
        } else if (ev.type == SDL_MOUSEBUTTONDOWN) {
          dragged = sys.ObjectFor(ev.button.x, ev.button.y);
          dragging = true;
        } else if (ev.type == SDL_MOUSEBUTTONUP) {
          dragging = false;
        } else if (ev.type == SDL_MOUSEMOTION && dragging) {
          dragged->Shift(ev.motion.xrel, ev.motion.yrel);
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


