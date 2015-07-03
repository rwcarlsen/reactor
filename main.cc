
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

const int neutron_burst = 5000;

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
    phys::BasicMaterial reflector1{0, 0, .1, 1, 0};
    phys::Object::Rect r1{w/2 - 40, h/2 - 40, 120, 120};
    reflector1.Init(r1, sdl::Color::white());
    phys::BasicMaterial reflector2(&reflector1);
    phys::BasicMaterial reflector3(&reflector1);
    phys::BasicMaterial reflector4(&reflector1);

    phys::Absorber absorber1{.1};
    phys::Object::Rect r2{w/2 - 130, h/2 - 40, 80, 80};
    absorber1.Init(r2, sdl::Color::blue());
    phys::Absorber absorber2(&absorber1);
    phys::Absorber absorber3(&absorber1);
    phys::Absorber absorber4(&absorber1);

    phys::BasicMaterial moderator1{0, 0, .03, .3, 0};
    phys::Object::Rect r3{w/2 + 50, h/2 - 40, 80, 80};
    moderator1.Init(r3, sdl::Color::green());
    phys::BasicMaterial moderator2(&moderator1);
    phys::BasicMaterial moderator3(&moderator1);
    phys::BasicMaterial moderator4(&moderator1);

    phys::Moderator voidmoderator1(.03, .3, 200);
    phys::Object::Rect r3b{w/2 + 130, h/2 - 40, 60, 60};
    voidmoderator1.Init(r3b, sdl::Color::lime());

    phys::Fuel fuel1(0, 0.061, 2);
    phys::Object::Rect r4{w/2 - 20, h/2 + 50, 40, 40};
    fuel1.Init(r4, sdl::Color::purple());
    phys::Fuel fuel2(&fuel1);
    phys::Fuel fuel3(&fuel1);
    phys::Fuel fuel4(&fuel1);
    phys::Fuel fuel5(&fuel1);
    phys::Fuel fuel6(&fuel1);
    phys::Fuel fuel7(&fuel1);
    phys::Fuel fuel8(&fuel1);
    phys::Fuel fuel9(&fuel1);

    phys::Object::Rect r5{w/2 + 130, h/2 + 40, 40, 40};
    phys::Detector detector1;
    detector1.Init(r5, sdl::Color::yellow());
    phys::Detector detector2(&detector1);
    phys::Detector detector3(&detector1);
    phys::Detector detector4(&detector1);
    
    phys::StreamSource stream1;
    phys::Object::Rect r6{w/2 + 130, h/2 - 40, 40, 40};
    stream1.Init(r6, sdl::Color::olive());

    // create system and a view for drawing it
    phys::System sys(w, h);
    sys.AddObject(&reflector1);
    sys.AddObject(&reflector2);
    sys.AddObject(&reflector3);
    sys.AddObject(&reflector4);
    sys.AddObject(&absorber1);
    sys.AddObject(&absorber2);
    sys.AddObject(&absorber3);
    sys.AddObject(&absorber4);
    sys.AddObject(&moderator1);
    sys.AddObject(&moderator2);
    sys.AddObject(&moderator3);
    sys.AddObject(&moderator4);
    sys.AddObject(&voidmoderator1);
    sys.AddObject(&fuel1);
    sys.AddObject(&fuel2);
    sys.AddObject(&fuel3);
    sys.AddObject(&fuel4);
    sys.AddObject(&fuel5);
    sys.AddObject(&fuel6);
    sys.AddObject(&fuel7);
    sys.AddObject(&fuel8);
    sys.AddObject(&fuel9);
    sys.AddObject(&detector1);
    sys.AddObject(&detector2);
    sys.AddObject(&detector3);
    sys.AddObject(&detector4);
    sys.AddObject(&stream1);
    stream1.sys(&sys);
    draw::SysView view(&sys, &ren);

    // start up the main loop
    SDL_Event ev;
    sdl::Timer timer;
    timer.Start();
    bool done = false;
    bool dragging = false;
    phys::Object* dragged = nullptr;
    phys::Object* clicked = nullptr;
    while(!done) {
      // process events
      while(SDL_PollEvent(&ev)) {
        if (ev.type == SDL_QUIT) {
          done = true;
        } else if (ev.type == SDL_MOUSEBUTTONDOWN &&
                   ev.button.button == SDL_BUTTON_RIGHT) {
          clicked = sys.ObjectFor(ev.button.x, ev.button.y);
          if (clicked->OnClick(ev.button.x, ev.button.y))
            continue;
          // create some neutrons
          phys::Neutron::Pop ns;
          for (int i = 0; i < neutron_burst; ++i) {
            double theta = uniform01(rand_gen) * 2 * 3.141592654;
            int vx = std::cos(theta) * phys::Neutron::kNomSpeed;
            int vy = std::sin(theta) * phys::Neutron::kNomSpeed;
            ns.push_back(phys::Neutron(ev.button.x, ev.button.y, vx, vy));
          }
          sys.AddNeutrons(ns);
        } else if (ev.type == SDL_MOUSEBUTTONDOWN && ev.button.button == SDL_BUTTON_LEFT) {
          dragged = sys.ObjectFor(ev.button.x, ev.button.y);
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
        } else if (ev.type == SDL_MOUSEBUTTONUP && ev.button.button == SDL_BUTTON_LEFT) {
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


