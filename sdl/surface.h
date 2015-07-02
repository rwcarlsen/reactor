
#ifndef SURFACE_H_
#define SURFACE_H_

#include "SDL2/SDL.h"
#include <memory>

#include "sdl/color.h"
#include "sdl/error.h"

namespace sdl {

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    const uint32_t R_MASK = 0xff000000;
    const uint32_t G_MASK = 0x00ff0000;
    const uint32_t B_MASK = 0x0000ff00;
    const uint32_t A_MASK = 0x000000ff;
#else
    const uint32_t R_MASK = 0x000000ff;
    const uint32_t G_MASK = 0x0000ff00;
    const uint32_t B_MASK = 0x00ff0000;
    const uint32_t A_MASK = 0xff000000;
#endif

class Surface {
 public:
  Surface(int width, int height) : surf_(nullptr) {
    SDL_DisplayMode curr;
    if (SDL_GetCurrentDisplayMode(0, &curr) != 0) {
      throw FatalErr();
    }
    fmt_ = SDL_AllocFormat(curr.format);

    std::cout << "r=" << fmt_->Rmask << ", a=" << fmt_->Amask << "\n";

    //surf_ = SDL_CreateRGBSurface(0, width, height, 32,
    //    R_MASK, G_MASK, B_MASK, A_MASK);
    surf_ = SDL_CreateRGBSurface(0, width, height, fmt_->BitsPerPixel,
                                 fmt_->Rmask, fmt_->Gmask,
                                 fmt_->Bmask, fmt_->Amask
                                );

    if (surf_ == nullptr) {
      throw FatalErr();
    }
    SDL_SetSurfaceBlendMode(surf_, SDL_BLENDMODE_BLEND);
  };

  Surface(SDL_Surface* surf) : surf_(surf) {
    SDL_DisplayMode curr;
    if (SDL_GetCurrentDisplayMode(0, &curr) != 0) {
      throw FatalErr();
    }
    fmt_ = SDL_AllocFormat(curr.format);
  };

  ~Surface() {
    SDL_FreeFormat(fmt_);
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
    uint32_t pix = SDL_MapRGBA(fmt_, c.r, c.g, c.b, c.a);
    SDL_Rect r = {p.x, p.y, 1, 1};
    if (SDL_FillRect(surf_, &r, pix) != 0) {
      throw FatalErr();
    }
  }

  void FillRect(const SDL_Rect* rect, Color c) {
    uint32_t pix = SDL_MapRGBA(fmt_, c.r, c.g, c.b, c.a);
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
  SDL_PixelFormat* fmt_;
};

} // namespace sdl

#endif
