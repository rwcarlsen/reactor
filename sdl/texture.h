
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "SDL2/SDL.h"

#include "error.h"
#include "renderer.h"
#include "sdl/surface.h"

namespace sdl {

class Texture {
 public:
  Texture() : ren_(nullptr), tex_(nullptr) { };

  Texture(const Renderer& ren, std::string bmp_file) {
    ren_ = ren.raw();

    SDL_Surface* bmp = SDL_LoadBMP(bmp_file.c_str());
    if (bmp == nullptr) {
      throw FatalErr();
    }

    tex_ = SDL_CreateTextureFromSurface(ren_, bmp);

    SDL_FreeSurface(bmp);
  };

  Texture(const Renderer& ren, const Surface& surf) {
    ren_ = ren.raw();
    tex_ = SDL_CreateTextureFromSurface(ren_, surf.raw());
  };

  ~Texture() {
    SDL_DestroyTexture(tex_);
  };

  Texture(const Texture& that) = delete;

  void ApplyFull(int x, int y) {
    if (ren_ == nullptr || tex_ == nullptr) {
      throw FatalErr("Uninitialized texture");
    }
    SDL_Rect pos = {x, y};
    SDL_QueryTexture(tex_, NULL, NULL, &pos.w, &pos.h);
    SDL_RenderCopy(ren_, tex_, NULL, &pos);
  };

  void ApplyRects(SDL_Rect* src, SDL_Rect* dst) {
    if (ren_ == nullptr || tex_ == nullptr) {
      throw FatalErr("Uninitialized texture");
    }
    SDL_RenderCopy(ren_, tex_, src, dst);
  };

 private:
  SDL_Renderer* ren_;
  SDL_Texture* tex_;
};

} // namespace sdl

#endif
