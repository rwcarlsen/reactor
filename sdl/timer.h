
#ifndef TIMER_H_
#define TIMER_H_

#include "SDL2/SDL.h"

namespace sdl {

class Timer {
 public:
  Timer() { };

  void Start() {
    start_ = SDL_GetTicks();
    mark_ = start_;
  }

  void Reset() {
    start_ = SDL_GetTicks();
    mark_ = start_;
  }

  int SinceStart() {
    return SDL_GetTicks() - start_;
  }

  int SinceMark() {
    return SDL_GetTicks() - mark_;
  }

  int Mark() {
    int curr = SDL_GetTicks();
    int delta = curr - mark_;
    mark_ = curr;
    return delta;
  };

 private:
  int start_;
  int mark_;
};

} // namespace sdl

#endif
