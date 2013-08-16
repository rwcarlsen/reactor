
#ifndef COLOR_H_
#define COLOR_H_

#include "SDL2/SDL.h"

namespace sdl {

class Color {
 public:
  Color() : r(0), g(0), b(0), a(SDL_ALPHA_OPAQUE) { };

  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) { };

  uint32_t pix() {
    SDL_DisplayMode curr;
    if (SDL_GetCurrentDisplayMode(0, &curr) != 0) {
      throw sdl::FatalErr();
    }
    SDL_PixelFormat* fmt = SDL_AllocFormat(curr.format);
    Color red = Color::red();
    Color blk = Color::black();
    uint32_t p =  SDL_MapRGB(fmt, r, g, b);
    SDL_FreeFormat(fmt);
    return p;
  }

#define COLOR(name, r, g, b) static Color name() {return Color(r, g, b, SDL_ALPHA_OPAQUE);}
COLOR(black, 0, 0, 0);
COLOR(white, 255, 255, 255);
COLOR(gray, 128, 128, 128);
COLOR(silver, 192, 192, 192);
COLOR(red, 255, 0, 0);
COLOR(lime, 0, 255, 0);
COLOR(blue, 0, 0, 255);
COLOR(yellow, 255, 255, 0);
COLOR(cyan, 0, 255, 255);
COLOR(magenta, 255, 0, 255);
COLOR(maroon, 128, 0, 0);
COLOR(green, 0, 128, 0);
COLOR(navy, 0, 0, 128);
COLOR(olive, 128, 128, 0);
COLOR(teal, 0, 128, 128);
COLOR(purple, 128, 0, 128);
#undef COLOR

  uint8_t r, g, b, a;
};

} // namespace

#endif
