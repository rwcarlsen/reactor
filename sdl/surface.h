
#ifndef SURFACE_H_
#define SURFACE_H_

#include "SDL2/SDL.h"
#include <memory>

#include "sdl/color.h"
#include "sdl/error.h"

namespace sdl {

class Surface {
 public:
  Surface(int width, int height) : surf_(nullptr) {
    surf_ = SDL_CreateRGBSurface(0, width, height, 32,
        R_MASK, G_MASK, B_MASK, A_MASK);

    if (surf_ == nullptr) {
      throw FatalErr();
    }
  };

  Surface(SDL_Surface* surf) : surf_(surf) { };

  ~Surface() {
    SDL_FreeSurface(surf_);
  };

  Surface(const Surface& that) = delete;

  SDL_Surface* raw() const {
    return surf_;
  };

  void Blit(Surface* other, SDL_Rect* at) {
    if ( SDL_BlitSurface(other->surf_, NULL, surf_, at) != 0) {
      throw FatalErr();
    }
  };

  void DrawPoint(SDL_Point p, Color c) {
    uint32_t pix = SDL_MapRGBA(surf_->format, c.r, c.g, c.b, c.a);
    SDL_Rect r = {p.x, p.y, 1, 1};
    if (SDL_FillRect(surf_, &r, pix) != 0) {
      throw FatalErr();
    }
  }

  void FillRect(const SDL_Rect* rect, Color c) {
    uint32_t pix = SDL_MapRGBA(surf_->format, c.r, c.g, c.b, c.a);
    if (SDL_FillRect(surf_, rect, pix) != 0) {
      throw FatalErr();
    }
  };

  void FillRectPix(const SDL_Rect* rect, uint32_t pix) {
    if (SDL_FillRect(surf_, rect, pix) != 0) {
      throw FatalErr();
    }
  };

  int width() {
    return surf_->w;
  };

  int height() {
    return surf_->h;
  };

 private:
  SDL_Surface* surf_;
};

} // namespace sdl

#endif
