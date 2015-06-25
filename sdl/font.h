
#ifndef FONT_H_
#define FONT_H_

#include <memory>

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include "sdl/error.h"

namespace sdl {

class Font {
 public:
  Font() : font_(nullptr) {};

  Font(std::string fontpath) throw(FatalErr) : font_(nullptr), fontpath_(fontpath) {
    Load(fontpath);
  };

  Font(const Font* other) : font_(nullptr) {
    fontpath_ = other->fontpath_;
    Load(fontpath_);
  };

  ~Font() {
    TTF_CloseFont(font_);
  };

  void Load(std::string fontpath, int size = 16) {
    font_ = TTF_OpenFont(fontpath.c_str(), size);
    if (font_ == nullptr) {
      throw FatalErr(std::string("Failed to load font ") + fontpath + ": " + TTF_GetError());
    }
  }

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
  std::string fontpath_;
};

} // namespace sdl

#endif
