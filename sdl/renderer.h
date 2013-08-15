
#ifndef RENDERER_H_
#define RENDERER_H_

#include "SDL2/SDL.h"

#include "color.h"
#include "error.h"

namespace sdl {

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

  void FillRect(const SDL_Rect* rect, Color c) {
    Color orig;
    if (SDL_GetRenderDrawColor(ren_, &orig.r, &orig.g, &orig.b, &orig.a) != 0) {
      throw FatalErr();
    }
    if (SDL_SetRenderDrawColor(ren_, c.r, c.g, c.b, c.a) != 0) {
      throw FatalErr();
    }
    if (SDL_RenderFillRect(ren_, rect) != 0) {
      throw FatalErr();
    }
    if (SDL_SetRenderDrawColor(ren_, orig.r, orig.g, orig.b, orig.a) != 0) {
      throw FatalErr();
    }
  };

  void Clear() {
    if (SDL_RenderClear(ren_) != 0) {
      throw FatalErr();
    }
  };

 private:
  SDL_Renderer* ren_;
};

} // namespace sdl

#endif
