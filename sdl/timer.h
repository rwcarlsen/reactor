
#ifndef TIMER_H_
#define TIMER_H_

#include "SDL2/SDL.h"

namespace sdl {

class Timer {
 public:
  Timer() : last_wait_(0), step_(100) { };

  void set_step(int msec) {
    step_ = msec;
  };

  void set_framerate(int fps) {
    step_ = 1000 / fps;
  };

  void Start() {
    last_wait_ = SDL_GetTicks();
  }

  void Wait() {
    int curr = SDL_GetTicks();
    int delta = curr - last_wait_;
    int wait = step_ - delta;
    if (wait > 0) {
      SDL_Delay(wait);
    }
  };

 private:
  int last_wait_;
  int step_;
};

} // namespace sdl

#endif
