
#ifndef RENDERER_H_
#define RENDERER_H_

#include "SDL2/SDL.h"

#include "sdl/window.h"
#include "sdl/color.h"
#include "sdl/error.h"

namespace sdl {

class Renderer {
 public:
  Renderer(const Window& w, uint32_t flags) : ren_(nullptr) {
    ren_ = SDL_CreateRenderer(w.raw(), -1, flags);
    if (ren_ == nullptr) {
      throw FatalErr();
    }

    SDL_BlendMode mode;
    int status = SDL_SetRenderDrawBlendMode(ren_, SDL_BLENDMODE_BLEND);
    if (status != 0) {
      throw FatalErr();
    }
  };

  ~Renderer() {
    SDL_DestroyRenderer(ren_);
  };

  Renderer(const Renderer& that) = delete;

  SDL_Renderer* raw() const {
    return ren_;
  };

  void set_draw_color(Color c) {
    if (SDL_SetRenderDrawColor(ren_, c.r, c.g, c.b, c.a) != 0) {
      throw FatalErr();
    }
  }

  void DrawPoints(SDL_Point* points, int npoints) {
    if (SDL_RenderDrawPoints(ren_, points, npoints) != 0) {
      throw FatalErr();
    }
  }

  SDL_Rect viewport() {
    SDL_Rect r;
    SDL_RenderGetViewport(ren_, &r);
    return r;
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

} // namespace sdl

#endif
