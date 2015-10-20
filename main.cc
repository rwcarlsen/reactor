
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
#include "phys/detector.h"
#include "phys/fuel_material.h"
#include "phys/moderator_material.h"
#include "phys/absorb_material.h"
#include "phys/stream_src.h"

#include "draw/sys_view.h"

using sdl::Color;

const int neutron_burst = 1000;

int main(int argc, char** argv) {
  std::ranlux48_base rand_gen;
  std::uniform_real_distribution<> uniform01;

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
    phys::Moderator voidmoderator1(.03, .3, 200);
    phys::Object::Rect r3b{w / 2 + 130, h / 2 - 40, 60, 60};
    voidmoderator1.Init(r3b, sdl::Color::lime());

    phys::Object::Rect r6{w / 2 + 80, h / 2 + 80, 50, 50};
    phys::Detector detector3;
    detector3.Init(r6, sdl::Color::yellow(128));
    phys::Detector detector4(&detector3);

    phys::StreamSource stream1;
    phys::Object::Rect r7{w / 2 + 130, h / 2 - 40, 40, 40};
    stream1.Init(r7, sdl::Color::olive());

    // create system and a view for drawing it
    phys::System sys(w, h);
    phys::Toolbar toolbar = sys.toolbar();

    phys::Object::Rect r;

    // add materials to system toolbar
    phys::Fuel fuel(0, 0.070, 2);
    r = {0, 0, 40, 40};
    fuel.Init(*toolbar.PlaceItem(&r), sdl::Color::purple());
    sys.AddObject(&fuel);

    phys::BasicMaterial reflector{0, 0, .1, 1, 0};
    r = {0, 0, 80, 80};
    reflector.Init(*toolbar.PlaceItem(&r), sdl::Color::white());
    sys.AddObject(&reflector);

    phys::Absorber absorber{.1};
    r = {0, 0, 80, 80};
    absorber.Init(*toolbar.PlaceItem(&r), sdl::Color::blue());
    sys.AddObject(&absorber);

    phys::BasicMaterial moderator{0, 0, .03, .3, 0};
    r = {0, 0, 80, 80};
    moderator.Init(*toolbar.PlaceItem(&r), sdl::Color::green());
    sys.AddObject(&moderator);

    phys::Detector detector;
    r = {0, 0, 100, 100};
    detector.Init(*toolbar.PlaceItem(&r), sdl::Color::yellow(128));
    sys.AddObject(&detector);

    draw::SysView view(&sys, &ren);

    // start up the main loop
    SDL_Event ev;
    sdl::Timer timer;
    timer.Start();
    bool done = false;
    bool dragging = false;
    phys::Object* dragged = nullptr;
    phys::Object* clicked = nullptr;
    while (!done) {
      // process events
      while (SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
          done = true;
        } else if (ev.type == SDL_MOUSEBUTTONDOWN &&
                   ev.button.button == SDL_BUTTON_RIGHT) {
          clicked = sys.ObjectFor(ev.button.x, ev.button.y);
          if (clicked->OnClick(ev.button.x, ev.button.y)) continue;
          // create some neutrons
          phys::Neutron::Pop ns;
          for (int i = 0; i < neutron_burst; ++i) {
            double theta = uniform01(rand_gen) * 2 * 3.141592654;
            int vx = std::cos(theta) * phys::Neutron::kNomSpeed;
            int vy = std::sin(theta) * phys::Neutron::kNomSpeed;
            ns.push_back(phys::Neutron(ev.button.x, ev.button.y, vx, vy));
          }
          sys.AddNeutrons(ns);
        } else if (ev.type == SDL_MOUSEBUTTONDOWN &&
                   ev.button.button == SDL_BUTTON_LEFT) {
          dragged = sys.ObjectFor(ev.button.x, ev.button.y);
          if (dragged->isToolbar()) {
            continue;
          }
          if (sys.InToolbar(dragged)) {
            phys::Object* temp = dragged->clone();
            sys.AddObject(&(*temp));
          }
          sys.MoveTop(dragged);
          dragging = true;
        } else if (ev.type == SDL_KEYDOWN) {
          if (dragged == nullptr) {
            continue;
          }
          if (ev.key.keysym.sym == SDLK_DOWN) {
            dragged->Shift(0, 5);
          } else if (ev.key.keysym.sym == SDLK_UP) {
            dragged->Shift(0, -5);
          } else if (ev.key.keysym.sym == SDLK_LEFT) {
            dragged->Shift(-5, 0);
          } else if (ev.key.keysym.sym == SDLK_RIGHT) {
            dragged->Shift(5, 0);
          }
        } else if (ev.type == SDL_MOUSEBUTTONUP &&
                   ev.button.button == SDL_BUTTON_LEFT) {
          if (sys.InToolbar(dragged)) {
            sys.RemoveObject(&(*dragged));
          }
          dragging = false;
        } else if (ev.type == SDL_MOUSEMOTION && dragging) {
          dragged->Shift(ev.motion.xrel, ev.motion.yrel);
        }
      }

      double dt = (double)timer.Mark();
      sys.Tick(dt / 1000);
      view.Render(((double)1000) / dt);
    }

    return 0;
  } catch (sdl::FatalErr err) {
    std::cout << "ERROR: " << err.what() << "\n";
    return 1;
  }
}
