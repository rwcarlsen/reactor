
#ifndef SDL_INIT_H_
#define SDL_INIT_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

namespace sdl {

class SDLinit {
 public:
  SDLinit(uint32_t flags = 0) throw(FatalErr) {
    if (SDL_Init(flags) != 0) {
      throw FatalErr();
    }
    if (TTF_Init() != 0) {
      throw FatalErr();
    }
  };

  ~SDLinit() {
    TTF_Quit();
    SDL_Quit();
  };

  SDLinit(const SDLinit& what) = delete;
};

} // namespace sdl

#endif
