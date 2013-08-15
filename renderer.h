
#ifndef RENDERER_H_
#define RENDERER_H_

#include "SDL2/SDL.h"

#include "error.h"

namespace rwc {

class Renderer {
 public:
  Renderer(SDL_Renderer* ren) : ren_(ren) { };

  SDL_Rect viewport() {
    SDL_Rect r;
    SDL_RenderGetViewport(ren_, &r);
    return r;
  };

  SDL_Renderer* raw() {
    return ren_;
  };

  void Render() {
    SDL_RenderPresent(ren_);
  };

  void Clear() {
    if (SDL_RenderClear(ren_) != 0) {
      throw FatalErr();
    }
  };

 private:
  SDL_Renderer* ren_;
};

} // namespace rwc

#endif
