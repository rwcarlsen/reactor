
#ifndef FONT_H_
#define FONT_H_

#include <memory>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "sdl/error.h"

namespace sdl {

class Font {
 public:
  Font(std::string font_path) throw(FatalErr) : font_(nullptr) {
    font_ = TTF_OpenFont(font_path.c_str(), 16);
    if (font_ == nullptr) {
      throw FatalErr(TTF_GetError());
    }
  };

  ~Font() {
    TTF_CloseFont(font_);
  };

  Font(const Font& what) = delete;

  std::unique_ptr<Surface> RenderSolid(std::string s, sdl::Color c) {
    SDL_Surface* surf = nullptr;
    surf = TTF_RenderText_Solid(font_, s.c_str(), c.sdl());
    if (surf == nullptr) {
      throw FatalErr(TTF_GetError());
    }
    return std::unique_ptr<Surface>(new Surface(surf));
  };

  std::unique_ptr<Surface> RenderBlended(std::string s, sdl::Color c) {
    SDL_Surface* surf = nullptr;
    surf = TTF_RenderText_Blended(font_, s.c_str(), c.sdl());
    if (surf == nullptr) {
      throw FatalErr(TTF_GetError());
    }
    return std::unique_ptr<Surface>(new Surface(surf));
  };

 private:
  TTF_Font* font_;
};

} // namespace sdl

#endif
