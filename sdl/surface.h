
#ifndef SURFACE_H_
#define SURFACE_H_

#include "SDL2/SDL.h"

#include "sdl/color.h"
#include "sdl/error.h"

namespace sdl {

class Surface {
 public:
  Surface(int width, int height) : surf_(nullptr) {
    SDL_DisplayMode curr;
    if (SDL_GetCurrentDisplayMode(0, &curr) != 0) {
      throw FatalErr();
    }
    fmt_ = SDL_AllocFormat(curr.format);

    surf_ = SDL_CreateRGBSurface(0, width, height, fmt_->BitsPerPixel,
                                 fmt_->Rmask, fmt_->Gmask,
                                 fmt_->Bmask, fmt_->Amask
                                );
    if (surf_ == nullptr) {
      throw FatalErr();
    }
  };

  ~Surface() {
    SDL_FreeFormat(fmt_);
    SDL_FreeSurface(surf_);
  };

  SDL_Surface* raw() const {
    return surf_;
  };

  void FillRect(const SDL_Rect* rect, Color c) {
    uint32_t pix = SDL_MapRGB(fmt_, c.r, c.g, c.b);
    if (SDL_FillRect(surf_, rect, pix) != 0) {
      throw FatalErr();
    }
  };

 private:
  SDL_Surface* surf_;
  SDL_PixelFormat* fmt_;
};

} // namespace sdl

#endif
