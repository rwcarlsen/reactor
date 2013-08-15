
#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "SDL2/SDL.h"

#include "error.h"

namespace rwc {

class Texture {
 public:
  Texture() : ren_(nullptr), tex_(nullptr) { };

  Texture(SDL_Renderer* ren, SDL_Surface* surf) {
    ren_ = ren;
    tex_ = SDL_CreateTextureFromSurface(ren_, surf);
  };

  ~Texture() {
    SDL_DestroyTexture(tex_);
  };

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

} // namespace rwc

#endif
