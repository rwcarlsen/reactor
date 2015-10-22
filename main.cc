
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

// returns false if the quit signal bas been sent.
bool ProcessEvents(phys::System* sys);

int main(int argc, char** argv) {

  try {
    sdl::SDLinit init(SDL_INIT_EVERYTHING);

    int w = 850;
    int h = 650;
    sdl::Window win("Reactor", w, h, 0);
    win.Center();
    win.Show();
    sdl::Renderer ren(win, SDL_RENDERER_ACCELERATED);

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

    phys::Moderator voidmoderator(.03, .3, 200);
    r = {0, 0, 60, 60};
    voidmoderator.Init(*toolbar.PlaceItem(&r), sdl::Color::lime());
    sys.AddObject(&voidmoderator);

    phys::StreamSource stream;
    r = {0, 0, 40, 40};
    stream.Init(*toolbar.PlaceItem(&r), sdl::Color::olive());
    sys.AddObject(&stream);

    draw::SysView view(&sys, &ren);

    // start up the main loop
    sdl::Timer timer;
    timer.Start();
    while (ProcessEvents(&sys)) {
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

bool ProcessEvents(phys::System* sys) {
  static phys::Object* dragged = nullptr;
  static std::ranlux48_base rand_gen;
  std::uniform_real_distribution<> uniform01;
  static bool lctrl = false;
  static bool rctrl = false;

  SDL_Event ev;
  while (SDL_PollEvent(&ev)) {
    if (ev.type == SDL_QUIT) {
      return false;
    } else if (ev.type == SDL_MOUSEBUTTONDOWN &&
               ev.button.button == SDL_BUTTON_RIGHT) {
      phys::Object* clicked = sys->ObjectFor(ev.button.x, ev.button.y);
      if (clicked->OnClick(ev.button.x, ev.button.y)) continue;
      // create some neutrons
      phys::Neutron::Pop ns;
      for (int i = 0; i < neutron_burst; ++i) {
        double theta = uniform01(rand_gen) * 2 * 3.141592654;
        int vx = std::cos(theta) * phys::Neutron::kNomSpeed;
        int vy = std::sin(theta) * phys::Neutron::kNomSpeed;
        ns.push_back(phys::Neutron(ev.button.x, ev.button.y, vx, vy));
      }
      sys->AddNeutrons(ns);
    } else if (ev.type == SDL_MOUSEBUTTONDOWN &&
               ev.button.button == SDL_BUTTON_LEFT) {
      dragged = sys->ObjectFor(ev.button.x, ev.button.y);
      if (dragged->isToolbar()) {
        continue;
      }
      if (sys->InToolbar(dragged)) {
        phys::Object* temp = dragged->clone();
        sys->AddObject(&(*temp));
      }
      sys->MoveTop(dragged);
    } else if (ev.type == SDL_MOUSEBUTTONUP &&
               ev.button.button == SDL_BUTTON_LEFT) {
      if (sys->InToolbar(dragged) && !dragged->isToolbar()) {
        sys->RemoveObject(&(*dragged));
      }
      dragged = nullptr;
    } else if (ev.type == SDL_MOUSEMOTION && dragged != nullptr) {
      if (dragged->isToolbar()) {
        continue;
      }
      if ((lctrl || rctrl) && !sys->InToolbar(dragged)) {
        dragged->Resize(ev.motion.x, ev.motion.y, ev.motion.xrel, ev.motion.yrel);
      } else {
        dragged->Shift(ev.motion.xrel, ev.motion.yrel);
      }
    } else if (ev.type == SDL_KEYDOWN) {
      if (ev.key.keysym.sym == SDLK_LCTRL) {
        lctrl = true; 
      } else if (ev.key.keysym.sym == SDLK_RCTRL) {
        rctrl = true; 
      } else {
	phys::Object* shifted = sys->objects().back();
	if (sys->InToolbar(shifted)) {
	  continue;
	}
        if (ev.key.keysym.sym == SDLK_DOWN) {
          shifted->Shift(0, 5);
        } else if (ev.key.keysym.sym == SDLK_UP) {
          shifted->Shift(0, -5);
        } else if (ev.key.keysym.sym == SDLK_LEFT) {
          shifted->Shift(-5, 0);
        } else if (ev.key.keysym.sym == SDLK_RIGHT) {
          shifted->Shift(5, 0);
        }
      }
    } else if (ev.type == SDL_KEYUP) {
      if (ev.key.keysym.sym == SDLK_LCTRL) {
        lctrl = false; 
      } else if (ev.key.keysym.sym == SDLK_RCTRL) {
        rctrl = false; 
      }
    }
  }
  return true;
}

